const fs = require('fs');
const path = require('path');

const configPath = path.join(__dirname, 'config.json');

let config = JSON.parse(fs.readFileSync(configPath, 'utf8'))

let email_username =  config.email.user;
let email_password = config.email.pass;

const ErrorCode = {
    Success: 0,
    Exception: 1
};

module.exports = {
    email_username,
    email_password,
    ErrorCode
}