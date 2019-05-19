package secureChat

import (
	"crypto/rsa"
	"encoding/json"
	"log"
)

type ClientInformation struct {
	PublicKey rsa.PublicKey
	Name      string
}

type ServerMessage struct {
	Sender string
	Type   string
	Data   []byte
}

type ClientMessage struct {
	Type string
	Data []byte
}

func ConstructServerMessage(sender, msgType string, message []byte) []byte {
	msg := ServerMessage{
		Sender: sender,
		Type:   msgType,
		Data:   message,
	}

	buff, err := json.Marshal(msg)
	if err != nil {
		log.Fatal(err)
	}

	return buff
}

func ConvertInServerMessage(originalMsg ClientMessage, sender string) ([]byte, error) {
	msg := ServerMessage{
		Sender: sender,
		Type:   originalMsg.Type,
		Data:   originalMsg.Data,
	}

	buff, err := json.Marshal(msg)
	if err != nil {
		return nil, err
	}

	return buff, nil
}

func ConstructClientMessage(msgType string, message []byte) ([]byte, error) {
	msg := ClientMessage{
		Type:   msgType,
		Data:   message,
	}

	buff, err := json.Marshal(msg)
	if err != nil {
		return nil, err
	}

	return buff, nil
}