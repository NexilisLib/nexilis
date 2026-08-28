# Dependencies

Nexilis depends on the boost library and OpenSSL.

We are using and linking the "system" and "json" parts of boost.

OpenSSL is used for two optional security features:

1. End-to-end message encryption (`nexilis/crypto.hh`): PBKDF2 key derivation and AES-256-GCM authenticated encryption.
2. TLS-PSK transport protection (`nexilis/tls.hh`): the whole TCP connection is encrypted with PSK ciphers derived from the authentication passphrase.

The library links against the full OpenSSL `libssl` (not just `libcrypto`).

## Downloading dependencies

For Linux download boost and openssl with your package manager.

## Arch Linux

```
pacman -S boost openssl
```

## Ubuntu

```
apt install libboost-all-dev libssl-dev
```

