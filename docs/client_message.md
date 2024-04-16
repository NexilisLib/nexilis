# Client message

Client message is in JSON format.

## Nexilis status

Client messages contain field "nexilis_status" that holds a value of integer.
Value of 1 indicates that the message is nexilis message and it's read and parsed by the server.
Value of 2 indicated that the message is nexilis message but it's not red nor parsed by the server.
New values might come up in the future.
