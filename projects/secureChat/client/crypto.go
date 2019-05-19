package main

import (
	"crypto/rand"
	"fmt"
	"encoding/json"
	"crypto/rsa"
	"crypto/sha256"
	"reflect"
)

type CryptoCoder struct {
	userKeys   map[string]rsa.PublicKey
	privateKey *rsa.PrivateKey
}


func (coder *CryptoCoder) encodeMessage(encode messageOnEncode) []byte {
	var msg encodedMessage

	label := []byte("")
	h := sha256.New()

	for _, recipient := range encode.recipients {
		if userPubKey, ok := coder.userKeys[recipient]; !ok {
			fmt.Println(recipient, " not added")
			continue
		} else {
			ciphertext, err := rsa.EncryptOAEP(
				h,
				rand.Reader,
				&userPubKey,
				[]byte(encode.msg),
				label,
			)
			if err != nil {
				fmt.Println(err)
				continue
			}

			msg.Parts = append(msg.Parts, ciphertext)
		}
	}
	ciphertext, err := rsa.EncryptOAEP(
		h,
		rand.Reader,
		&coder.privateKey.PublicKey,
		[]byte(encode.msg),
		label,
	)
	if err != nil {
		fmt.Println(err)
	} else {
		msg.Parts = append(msg.Parts, ciphertext)
	}

	msg.H = h.Sum(nil)

	buff, err := json.Marshal(msg)
	if err != nil {
		fmt.Println(err)
	}

	return buff
}

func (coder *CryptoCoder) decodeMessage(serializedEncodedMessage []byte) string {
	label := []byte("")
	h := sha256.New()

	var msg encodedMessage
	err := json.Unmarshal(serializedEncodedMessage, &msg)
	if err != nil {
		fmt.Println(err)
		return ""
	}

	for _, part := range msg.Parts {
		plainText, err := rsa.DecryptOAEP(
			h,
			rand.Reader,
			coder.privateKey,
			part,
			label,
		)
		if err != nil {
			continue
		}

		if reflect.DeepEqual(msg.H, h.Sum(nil)) {
			return string(plainText)
		}
	}
	return "unable to decrypt"
}