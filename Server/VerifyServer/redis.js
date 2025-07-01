const config = require('./config');
const Redis = require('ioredis');

//Create redis client
const redisClient = new Redis({
    host: config.redis_host, 
    port: config.redis_port,
    password: config.redis_passowrd
});

redisClient.on("error", function(err) {
    console.log("Failed to connect redis client");
});

async function GetRedis(key) {
    try {
        const result = await redisClient.get(key);
        if (result == null) {
            console.log('Failed to get key: ' + key);
            return null;
        }

        console.log('Got key: ' + key);
        return result;
        
    }catch (error) {
        console.log('GetRedis error: ' + error);
        return null
    }
}

async function ExistsKey(key) {
    try {
        const result = await redisClient.exists(key);
        if (result == 0) {
            console.log('Not found key: ' + key);
            return null;
        }

        console.log('Found key: ' + key);
        return result;
        
    } catch (error) {
        console.log('ExistsKey error: ' + error);
    }
}

async function SetKeyExpired(key, value, expiredTime) {
    try {
        await redisClient.set(key, value);
        await redisClient.expire(key, expiredTime);
        return true;
    } catch (error) {
        console.log('SetKeyExpired error: ' + error);
    }
}

function Quit() {
    redisClient.quet();
}

module.exports = {GetRedis, ExistsKey, SetKeyExpired, Quit}