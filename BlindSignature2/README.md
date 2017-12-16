# Compilation
`./start.sh`

# server setup mode
`./bserver setup folder_to_save_keys`
(e.g. `./bserver setup key/`)

# server sign mode
`./bserver sign ./bserver sign 'server_key' port path_to_private_key`
(e.g. `./bserver sign 'l1SzN#4CjGbZsJR!' 8080 key/private2048`)

# client
`./bclient port path_to_public_key msg`
(e.g. `./bclient 8080 key/public2048 x`)

