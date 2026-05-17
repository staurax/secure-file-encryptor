# Secure File Encryptor

A modern Windows desktop tool for encrypting and decrypting local files with password protection.

Built with C++ and Qt 6 Widgets.

## Features

- File encryption and decryption
- Password-protected encrypted files
- Local processing only
- Modern two-panel Qt desktop interface
- Portable Windows release package

## Download

Download the latest Windows build from the GitHub Releases page.

Use the full portable folder from the `.zip` file. Do not run the `.exe` by itself because the Qt runtime files must stay beside it.

## Author

Build by Hannan Asghar (@Staurax)

## Project Notes

This project preserves the original Secure File Encryptor V2 backend logic:

- `MAGIC_HEADER = "ENC_V2"`
- `KEY_STRETCH = 1000`
- `passwordChecksum()`
- `stretchKey()`
- `xorCrypt()`
- `encryptFile()`
- `decryptFile()`

