'use strict';

var cradle = require('cradle');
var _ = require('underscore');
var Promise = require('promise');

var host = 'http://localhost';
var port = 5994;
var url = host + ':' + port;
var conn = new cradle.Connection(host, port, { cache: false });

var testDocument = { 'lorem' : 'ipsum', pi: 3.14159, sunny: true, free_lunch: false, the_answer: 42, 
    taxRate: null, fibonnaci: [0, 1, 1, 2, 3, 5, 8, 13 ], child: { 'hello': 'world' }, 
    events: [ null, 1969, 'avance', true, {}, [] ], //minNUm: Number.MIN_VALUE , maxNum: Number.MAX_VALUE,
    data: 
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789' + 
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789' +
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789' +
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789' +
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789' +
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789' +
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789' +
    '0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789',
    formatting: '\r\n\t\f\b\\/"' };

var testDbName = 'avancedb-loadtest';
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

var init = new Promise(function(resolve, reject) {
    db.destroy(function() {
        db.create(function(err, res) {
            if (err) {
                reject(err);
            } else {
                resolve();
            }
        });
    });
});

init.then(function() { 
    var count = 1000;
    var active = 0;
    var index = 0;
    
    var save = function(next) {
        ++active;
        --count;
        if (count >= 0) {
            var testData  = [];
            for (var i = 0; i < 1000; ++i) {
                testData[i] = _.extend({index: index, _id: '' + (index++)}, testDocument);
            }
            
            db.save(testData, function(err, res) {
                --active;
                if (err) throw err;
                next(next);
            });
            
            if (active < 10) {
                next(next);
            }
        }
    };
    
    save(save);
});
