const fs = require('fs');
const path = require('path');

const configPath = path.join(__dirname, 'config.json');

let config = JSON.parse(fs.readFileSync(configPath, 'utf8'))

let email_username = config.email.user;
let email_password = config.email.password;

let redis_host = config.redis.host;
let redis_port = config.redis.port;
let redis_passowrd = config.redis.password;

const ErrorCode = {
    Success: 0,
    Exception: 1,
    RedisError: 3
};

//let code_prefix = "code_"

module.exports = {
    email_username,
    email_password,
    redis_host,
    redis_port,
    redis_passowrd,
    //code_prefix,
    ErrorCode
}