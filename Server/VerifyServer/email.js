const nodemailer = require('nodemailer')
const config = require('./config')

let transport = nodemailer.createTransport(
    {
        host: 'smtp.163.com',
        port: 465,
        secure: true, // true for 465, false for other ports
        auth: {
            user: config.email_username,
            pass: config.email_password
        }
    }
)

function sendEmail(emailOptions) {
    return new Promise(function (resolve, reject){
        transport.sendMail(emailOptions, function (err, info) {
            if (err) {
                console.error('Error sending email:', err)
                reject(err)
            } else {
                console.log('Email sent successfully:', info.response)
                resolve(info)
            }
        })
    });
}

module.exports = { sendEmail}
