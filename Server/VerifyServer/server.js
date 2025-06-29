const email = require('./email');
const grpc = require('@grpc/grpc-js');
const messageProto = require('./proto');
const config = require('./config')
const { v4: uuidv4 } = require('uuid');

async function GetVerifyCode(call, callback) {
    try {
        uuid = uuidv4().substring(0, 4);
        console.log('VerifyCode:', uuid);
        const emailOptions = {
            from: config.email_username,
            to: call.request.email,
            subject: 'Your Verification Code',
            text: `Your verification code is: ${uuid}`
        };

        let result = await email.sendEmail(emailOptions);
        console.log(result);
        callback(null,  {email: call.request.email, error: config.ErrorCode.Success, code: uuid});

    }catch (error) {
        console.error('Error in GetVerifyCode:', error);
        callback({
            email: call.request.email,
            error: config.ErrorCode.Exception
        });
    }
}

function main() {
    server =  new grpc.Server();
    server.addService(messageProto.VerifyService.service, {GetVerifyCode : GetVerifyCode});
    server.bindAsync('0.0.0.0:50051', grpc.ServerCredentials.createInsecure(), () => {
        //server.start();
        console.log('Server running at http://0.0.0.0:50051');
    });
}

main()