const email = require('./email');
const grpc = require('@grpc/grpc-js');
const messageProto = require('./proto');
const config = require('./config')
const { v4: uuidv4 } = require('uuid');
const redis = require('./redis')

async function GetVerifyCode(call, callback) {
    try {
        //emailname:uuid
        const queryKey = call.request.email;
        let verifyCode = await redis.GetRedis(queryKey);
        if (verifyCode == null)
        {
            verifyCode = uuidv4().substring(0, 4);
            //Set expired time in redis
            let res = await redis.SetKeyExpired(queryKey, verifyCode, 600);
            if (!res) {
                callback(null, {email: call.request.email, error: config.ErrorCode.RedisError});
                return;
            }
        }

        console.log('VerifyCode:', verifyCode);
        const emailOptions = {
            from: config.email_username,
            to: call.request.email,
            subject: 'Your Verification Code',
            text: `Your verification code is: ${verifyCode}`
        };

        //let result = await email.sendEmail(emailOptions);
        //console.log(result);
        callback(null,  {email: call.request.email, error: config.ErrorCode.Success, code: verifyCode});

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
    server.bindAsync('0.0.0.0:50052', grpc.ServerCredentials.createInsecure(), () => {
        //server.start();
        console.log('Server running at http://0.0.0.0:50052');
    });
}

main()