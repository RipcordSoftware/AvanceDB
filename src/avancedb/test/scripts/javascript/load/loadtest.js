'use strict';

var http = require('http');
var cradle = require('cradle');
var _ = require('underscore');
var Promise = require('promise');
var program = require('commander');
var msgpack = require("msgpack-lite");

program
  .version('0.0.1')
  .option('-n, --name [name=avancedb-loadtest]', 'The name of the database to create', 'avancedb-loadtest')
  .option('-c, --count [records=1000000]', 'The number of records to add to the database', Number, 1000000)
  .option('-b, --block [size=2000]', 'The number of records in each block', Number, 2000)
  .option('-s, --step [size=1]', 'The step to use when incrementing record ids', Number, 1)
  .option('-o, --offset [offset=0]', 'The offset from 0 to use for ids', Number, 0)
  .option('-a, --anonymous', 'Add records without an _id field', false)
  .option('-u, --update', 'Update the database if it already exists, don\'t drop it first', false)
  .option('-p, --port [port=5994]', 'The port number to connect to', Number, 5994)
  .option('-h, --host [name=127.0.0.1]', 'The IP or hostname to connect to', '127.0.0.1')
  .option('-m, --msgpack', 'Use msgpack instead of JSON for bulk POSTs', false)
  .parse(process.argv);

var host = 'http://' + program.host;
var port = program.port;
var url = host + ':' + port;
var conn = new cradle.Connection(host, port, { cache: false });

var testDocument = { 'lorem' : 'ipsum', pi: 3.14159, sunny: true, free_lunch: false, the_answer: 42, 
    taxRate: null, fibonnaci: [0, 1, 1, 2, 3, 5, 8, 13 ], child: { 'hello': 'world' }, 
    events: [ null, 1969, 'avance', true, {}, [] ], //minNUm: Number.MIN_VALUE , maxNum: Number.MAX_VALUE,
    data: 
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789' +
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789' +
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789' +
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789',
    formatting: '\r\n\t\f\b\\/"' };

var testDbName = program.name;
var db = conn.database(testDbName);

var save = new Promise(function(resolve, reject) {
    db.save(testData, function(err, res) {
        if (err) {
            reject(err);
        } else {
            resolve();
        }
    });
});

var dbSave = function(data, cb, useMsgPack) {
    if (!useMsgPack) {
        db.save(data, cb);
    } else {
        try {
            var body = {docs: data};
            var buffer = msgpack.encode(body);

            var options = {headers: {}};
            options.family = 4;
            options.host = conn.host;
            options.port = conn.port;
            options.path = '/' + testDbName + '/_bulk_docs';
            options.method = 'POST';
            options.headers['Content-Type'] = 'application/msgpack';
            options.headers['Content-Length'] = buffer.length;

            var req = http.request(options, function(res) {
                res.on('data', function (chunk) {
                });
                res.on('end', function () {
                    cb(null, null);
                });
            });
            req.on('error', function(e) { cb(e, null); });
            req.write(buffer);
            req.end();
        } catch (e) {
            console.log(e);
        }
    }
}

var init = new Promise(function(resolve, reject) {
    db.exists(function(err, exists) {
        if (err) {
            reject(err);
        } else if (exists && program.update) {
            resolve();
        } else {
            db.destroy(function(err, res) {
                if (exists && err) {
                    reject(err);
                }

                db.create(function(err, res) {
                    if (err) {
                        reject(err);
                    } else {
                        resolve();
                    }
                });
            });
        } 
    });
});

init.then(function() {
    var count = Math.floor(program.count / program.block);
    var overflow = program.count % program.block;
    var active = 0;
    var index = program.offset;
    var start = process.hrtime();
    
    var save = function(next) {
        if (count >= 0 || overflow > 0) {
            ++active;
            --count;

            var blockSize = program.block;
            if (count < 0) {
                blockSize = overflow;
                overflow = 0;
            }

            var testData  = [];
            for (var i = 0; i < blockSize; ++i) {
                testData[i] = program.anonymous ? testDocument : _.extend({index: index, _id: ('000000000000' + index).slice(-12)}, testDocument);
                index += program.step;
            }
            
            dbSave(testData, function(err, res) {
                --active;
                if (err) throw err;
                process.stdout.write('.');
                next(next);
            }, program.msgpack);
            
            if (active < 10) {
                next(next);
            }
        } else if (active == 0) {
            var end = process.hrtime();            
            var time = (end[0] * 1000) + (end[1] / 10000000);
            time -= (start[0] * 1000) + (start[1] / 10000000);
            console.log('\nTime: ' + Math.round(time) + 'ms');
        }
    };
    
    save(save);
});
