package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"encoding/json"
	"crypto/rsa"
	"os"
	"github.com/Jenkslex/secureChat"
	"flag"
	"math/rand"
	"time"
	"crypto/sha256"
	crand "crypto/rand"
	"reflect"
)


type clientChan chan []byte

var (
	letterRunes = []rune("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")

	serverAddress  = flag.String("address", "127.0.0.1:8000", "address of the server")

	enteringUser   = make(chan string)
	enteringChan   = make(chan clientChan)
	enteringStatus = make(chan error)

	leaving  = make(chan string)
	messages = make(chan []byte)

	registering      = make(chan string)
	registeringKey   = make(chan rsa.PublicKey)
	registeringStatus = make(chan error)

	authorizeKey     = make(chan rsa.PublicKey)
	authorizeUser    = make(chan string)
)

func randStringRunes(n int) string {
	b := make([]rune, n)
	for i := range b {
		b[i] = letterRunes[rand.Intn(len(letterRunes))]
	}
	return string(b)
}

func broadcaster() {
	registeredUsers := make(map[rsa.PublicKey]string)
	clientsInfo     := make(map[string][]byte)

	connections     := make(map[string]clientChan)

	for {
		select {
		case newKey := <- authorizeKey:
			founded := false
			for k, v := range registeredUsers {
				if reflect.DeepEqual(k.N, newKey.N) {
					authorizeUser <- v
					founded = true
					break
				}
			}
			if !founded {
				authorizeUser <- ""
			}

		case newKey := <- registeringKey:
			newUser := <- registering

			registeredUsers[newKey] = newUser

			client := secureChat.ClientInformation{
				PublicKey: newKey,
				Name:      newUser,
			}

			buff, err := json.Marshal(client)
			if err != nil {
				continue
			}
			msg := secureChat.ConstructServerMessage(*serverAddress, "pubKey", buff)
			clientsInfo[newUser] = msg

			for _, cli := range connections {
				if cli != nil {
					cli <- msg
				}
			}

		case msg := <-messages:
			for _, cli := range connections {
				if cli != nil {
					cli <- msg
				}
			}

		case userName := <- enteringUser:
			if _, ok := clientsInfo[userName]; !ok {
				enteringStatus <- fmt.Errorf("%s not registred", userName)
				continue
			}
			if connections[userName] != nil {
				enteringStatus <- fmt.Errorf("%s has already connected", userName)
				continue
			}
			enteringStatus <- nil

			clientChannel := <- enteringChan

			for _, info := range clientsInfo {
				clientChannel <- info
			}

			connections[userName] = clientChannel

		case userName := <- leaving:
			cli := connections[userName]
			close(cli)
			connections[userName] = nil
		}
	}
}

func keyResponse(input *bufio.Scanner, conn net.Conn) (*rsa.PublicKey, error) {
	var buff []byte

	if input.Scan() {
		buff = input.Bytes()
	} else {
		return nil, fmt.Errorf("client disconnected")
	}

	var publicKey rsa.PublicKey
	err := json.Unmarshal(buff, &publicKey)
	if err != nil {
		return nil, fmt.Errorf("failed to unmarshal public key <%x>: %v",
			buff, err)
	}

	randomString := randStringRunes(10)

	label := []byte("")
	h := sha256.New()

	cipherText, err := rsa.EncryptOAEP(
		h,
		crand.Reader,
		&publicKey,
		[]byte(randomString),
		label,
	)
	if err != nil {
		return nil, err
	}

	conn.Write(cipherText)

	if input.Scan() {
		buff = input.Bytes()
	} else {
		return nil, fmt.Errorf("client disconnected")
	}

	if randomString != string(buff) {
		return nil, fmt.Errorf("client has not private key")
	}

	return &publicKey, nil
}

func authorization(conn net.Conn) (string, error) {
	input := bufio.NewScanner(conn)

	publicKey, err := keyResponse(input, conn)
	if err != nil {
		return "", fmt.Errorf("authorization error %v", err)
	}

	authorizeKey <- *publicKey
	userName := <- authorizeUser

	if userName != "" {
		fmt.Fprintln(conn, "OK")
		return userName, nil
	}
	fmt.Fprintln(conn, "UserName")

	var nickName []byte
	if input.Scan() {
		nickName = input.Bytes()
	} else {
		return "", fmt.Errorf("client disconnected")
	}

	registeringKey <- *publicKey
	registering    <- string(nickName)

	return string(nickName), nil
}


func handleUser(userName string, netConnection net.Conn) {
	ch := make(chan []byte)
	enteringUser <- userName
	status := <- enteringStatus
	if status != nil {
		fmt.Println(status)
		return
	}
	enteringChan <- ch

	go clientWriter(netConnection, ch)

	input := bufio.NewScanner(netConnection)

	ch <- secureChat.ConstructServerMessage(*serverAddress, "public",
		[]byte("You are " + userName))
	messages <- secureChat.ConstructServerMessage(*serverAddress, "public",
		[]byte(userName + " has arrived"))

	for input.Scan() {
		serializedMessage := input.Bytes()

		if len(serializedMessage) != 0 {
			var message secureChat.ClientMessage
			err := json.Unmarshal(serializedMessage, &message)
			if err != nil {
				fmt.Println("Failed to unmarshal msg ", serializedMessage, err)
				continue
			}

			serverMsg, err := secureChat.ConvertInServerMessage(message, userName)
			if err != nil {
				fmt.Println("Failed to conver message ", message, err)
				continue
			}

			messages <- serverMsg
		}
	}

	leaving <- userName

	messages <- secureChat.ConstructServerMessage(*serverAddress, "public",
		[]byte(userName + " has left"))
}

func clientWriter(conn net.Conn, ch <-chan []byte) {
	for msg := range ch {
		fmt.Fprintln(conn, string(msg))
	}
}

func handleConn(netConnection net.Conn) {
	defer netConnection.Close()

	who, err := authorization(netConnection)
	if err != nil {
		fmt.Fprintf(os.Stderr,"%v", err)
		return
	}

	handleUser(who, netConnection)
}

func main() {
	flag.Parse()
	rand.Seed(time.Now().UnixNano())

	listener, err := net.Listen("tcp", *serverAddress)
	if err != nil {
		log.Fatal(err)
	}

	go broadcaster()
	for {
		newNetConnection, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(newNetConnection)
	}
}