package main

import (
	"log"
	"net"
	"os"
	"crypto/rsa"
	"crypto/rand"
	"fmt"
	"encoding/json"
	"bufio"

	"github.com/Jenkslex/secureChat"
	"flag"
	"crypto/sha256"
	"io/ioutil"
)

type messageOnEncode struct {
	msg        string
	recipients []string
}

type encodedMessage struct {
	Parts [][]byte  `json:"parts"`
	H     []byte    `json:"h"`
}

var (
	serverAddress  = flag.String("server", "127.0.0.1:8000", "server address")
	privateKeyPath = flag.String("key", "key", "path to the file with private key")

	entering = make(chan secureChat.ClientInformation)

	encoding = make(chan messageOnEncode)
	encoded  = make(chan []byte)

	decoding = make(chan []byte)
	decoded  = make(chan string)
)


func coder(myPrivateKey *rsa.PrivateKey) {
	crypto := CryptoCoder {
		userKeys:   make(map[string]rsa.PublicKey),
		privateKey: myPrivateKey,
	}

	for {
		select {
		case clientInfo := <- entering:
			fmt.Printf("Get new pubKey for user %s\n", clientInfo.Name)
			crypto.userKeys[clientInfo.Name] = clientInfo.PublicKey

		case msgOnEncode := <- encoding:
			encoded <- crypto.encodeMessage(msgOnEncode)

		case msgOnDecode := <- decoding:
			decoded <- crypto.decodeMessage(msgOnDecode)
		}
	}
}

func readBytes(conn net.Conn, n int) ([]byte, error) {
	buff := make([]byte, 0, n) // big buffer
	tmp  := make([]byte, 256)     // using small tmo buffer for demonstrating

	fullRead := 0
	for {
		read, _ := conn.Read(tmp)

		fullRead += read
		buff = append(buff, tmp[:read]...)

		if fullRead < n {
			continue
		}

		break
	}
	return buff, nil
}

func keyResponse(conn net.Conn, privateKey *rsa.PrivateKey) error {
	publicKey := &privateKey.PublicKey

	buff, err := json.Marshal(publicKey)
	if err != nil {
		return fmt.Errorf("keyResponse error: %v", err)
	}

	fmt.Fprintln(conn, string(buff))

	cipherText, err := readBytes(conn, 256)

	label := []byte("")
	h := sha256.New()

	plainText, err := rsa.DecryptOAEP(
		h,
		rand.Reader,
		privateKey,
		cipherText,
		label,
	)
	if err != nil {
		return fmt.Errorf("keyResponse error: %v", err)
	}

	fmt.Fprintln(conn, string(plainText))

	return nil
}


func authorization(conn net.Conn, privateKey *rsa.PrivateKey) {
	err := keyResponse(conn, privateKey)

	reader := bufio.NewReader(conn)
	status, err := reader.ReadString('\n')
	if err != nil {
		log.Fatal("Kek")
	}
	status = status[:len(status) - 1]

	if status == "OK" {
		return
	} else if status == "UserName"{
		fmt.Println("Enter your nickName")

		reader := bufio.NewReader(os.Stdin)
		userName, _ := reader.ReadString('\n')

		fmt.Fprintln(conn, userName[:len(userName) - 1])

	} else {
		log.Fatal("Not autorized")
	}
}

func loadPrivateKey(privateKeyPath string) (*rsa.PrivateKey, error) {
	rawKey, err := ioutil.ReadFile(privateKeyPath)
	if err != nil {
		privateKey, err := rsa.GenerateKey(rand.Reader, 2048)
		if err != nil {
			return nil, err
		}

		privateKeyJson, _ := json.Marshal(privateKey)
		err = ioutil.WriteFile(privateKeyPath, privateKeyJson, 0644)

		fmt.Println("New key has been generated")

		return privateKey, err

	} else {
		var privateKey rsa.PrivateKey
		err = json.Unmarshal(rawKey, &privateKey)
		if err != nil {
			return nil, err
		}

		fmt.Println("Key has been loaded from file")

		return &privateKey, nil
	}
}


func handleConn(conn net.Conn, privateKey *rsa.PrivateKey, done chan struct{}) {
	authorization(conn, privateKey)

	defer conn.Close()

	go coder(privateKey)
	go sending(conn)

	reader := bufio.NewReader(conn)
	for {
		msg, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Failed to read from connection", err)
			break
		}

		serializedMessage := []byte(msg)

		var message secureChat.ServerMessage
		err = json.Unmarshal(serializedMessage, &message)
		if err != nil {
			fmt.Println("Failed to unmarshal msg ", serializedMessage, err)
			continue
		}

		switch message.Type {
		case "public":
			if message.Sender == *serverAddress {
				fmt.Println("Admin: " + string(message.Data))
			} else {
				fmt.Println(message.Sender + ": " + string(message.Data))
			}

		case "private":
			decoding <- message.Data
			decodedMessage := <- decoded
			fmt.Println(message.Sender + ": " + decodedMessage)

		case "pubKey":
			var clientInfo secureChat.ClientInformation
			err := json.Unmarshal(message.Data, &clientInfo)
			if err != nil {
				fmt.Println("Failed to reset unmarshal client info ", message.Data, err)
				continue
			}
			entering <- clientInfo
		}
	}

	log.Println("done")
	done <- struct{}{}
}

func scanRecipients(scanner* bufio.Scanner) []string {
	var recipients []string
	for scanner.Scan() {
		text := scanner.Text()

		if text == "%w" {
			break
		}
		recipients = append(recipients, text)
	}
	return recipients
}

func sending(conn net.Conn) {
	var err error
	var msg []byte

	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		text := scanner.Text()

		if text == "%q" {
			recipients := scanRecipients(scanner)
			for scanner.Scan() {
				text = scanner.Text()
				break
			}

			privateMsg := messageOnEncode{
				recipients: recipients,
				msg:        text,
			}
			encoding <- privateMsg
			encodedMsg := <- encoded


			msg, err = secureChat.ConstructClientMessage("private", encodedMsg)
		} else {
			msg, err = secureChat.ConstructClientMessage("public", []byte(text))
		}
		if err != nil {
			continue
		}

		fmt.Fprintln(conn, string(msg))
	}
}

func main() {
	flag.Parse()

	privateKey, err := loadPrivateKey(*privateKeyPath)
	if err != nil {
		log.Fatal(err, ": ", *privateKeyPath)
	}

	conn, err := net.Dial("tcp", *serverAddress)
	if err != nil {
		log.Fatal(err, ": ", *serverAddress)
	}

	done := make(chan struct{})
	handleConn(conn, privateKey, done)

	fmt.Println(conn.LocalAddr().String())

	sending(conn)
	conn.Close()
	<-done
}
