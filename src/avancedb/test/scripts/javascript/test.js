'use strict';

var assert = require('assert');
var cradle = require('cradle');
var _ = require('underscore');
var http = require('http');

var host = 'http://localhost';
var port = 15994;
//var port = 5994;
var url = host + ':' + port;
var conn = new cradle.Connection(host, port, { cache: false });

describe('avancedb -- server info --', function() {
    it('should validate server signature', function(done) {
        conn.info(function (err, res) {
            assert.equal(null, err);
            assert.deepEqual(res, {
                "couchdb":"Welcome",
                "avancedb":"Welcome",
                "uuid":"a2db86472466bcd02e84ac05a6c86185",
                "version":"1.6.1",
                "vendor":{
                    "version":"0.0.1",
                    "name":"Ripcord Software"
                }
            });
            done();
        });
    });
});

describe('avancedb -- futon --', function() {
    it('should get a redirect', function(done) {
        http.get(url + '/_utils', function(res) {
            assert.equal(307, res.statusCode);
            assert.equal('/_utils/index.html', res.headers.location);
            res.on('data', function(){});
            done();
        });
    });    
    
    it('should get html', function(done) {
        http.get(url + '/_utils/index.html', function(res) {
            assert.equal(200, res.statusCode);
            assert.equal('text/html', res.headers['content-type']);
            res.on('data', function(){});
            done();
        });
    });
    
    it('should get the favicon', function(done) {
        http.get(url + '/_utils/favicon.ico', function(res) {
            assert.equal(200, res.statusCode);
            assert.equal('image/ico', res.headers['content-type']);
            res.on('data', function(){});
            done();
        });
    });
    
    it('should get the logo', function(done) {
        http.get(url + '/_utils/image/logo.png', function(res) {
            assert.equal(200, res.statusCode);
            assert.equal('image/png', res.headers['content-type']);
            res.on('data', function(){});
            done();
        });
    });
    
    it('shouldn\'t get a valid response', function(done) {
        http.get(url + '/_utils/nothing_to_see_here', function(res) {
            assert.equal(404, res.statusCode);
            assert.equal(0, res.headers['content-length']);
            res.on('data', function(){});
            done();
        });
    });
});

describe('avancedb -- config --', function() {
    it('should get the query server config', function(done) {
        http.get(url + '/_config/query_servers', function(res) {
            assert.equal(200, res.statusCode);
            assert.equal('application/json', res.headers['content-type']);
            res.on('data', function(chunk) {
                var config = JSON.parse(chunk);
                assert.notEqual(null, config.javascript);
                done();
            });
        });
    });    
    
    it('should get the native query server config', function(done) {
        http.get(url + '/_config/native_query_servers', function(res) {
            assert.equal(200, res.statusCode);
            assert.equal('application/json', res.headers['content-type']);
            res.on('data', function(chunk) {
                assert.equal('{}', chunk);
                done();
            });
        });
    });
});

describe('avancedb -- tasks --', function() {
    it('should get no tasks', function(done) {
        conn.activeTasks(function(err, tasks) {
            assert.equal(null, err);
            assert.notEqual(null, tasks);
            assert.equal(0, tasks.length);
            done();
        });
    });
});

describe('avancedb -- uuids --', function() {
    it('should get one uuid', function(done) {
        conn.uuids(1, function(err, uuids) {
            assert.equal(null, err);
            assert.notEqual(null, uuids);
            assert.equal(1, uuids.length);
            done();
        });
    });
    
    it('ask for zero uuids get one (the default)', function(done) {
        conn.uuids(0, function(err, uuids) {
            assert.equal(null, err);
            assert.notEqual(null, uuids);
            assert.equal(1, uuids.length);
            done();
        });
    });
    
    it('should get minus one uuid', function(done) {
        conn.uuids(-1, function(err, uuids) {
            assert.equal(null, err);
            assert.notEqual(null, uuids);
            assert.equal(0, uuids.length);
            done();
        });
    });
    
    it('should get one hundred uuids', function(done) {
        conn.uuids(100, function(err, uuids) {
            assert.equal(null, err);
            assert.notEqual(null, uuids);
            assert.equal(100, uuids.length);
            done();
        });
    });
    
    it('should get one thousand uuids', function(done) {
        conn.uuids(1000, function(err, uuids) {
            assert.equal(null, err);
            assert.notEqual(null, uuids);
            assert.equal(1000, uuids.length);
            done();
        });
    });
    
    it('should get ten thousand uuids', function(done) {
        conn.uuids(10000, function(err, uuids) {
            assert.notEqual(null, err);
            assert.equal('forbidden', err.error);
            assert.equal(403, err.headers.status);
            assert.equal(null, uuids);
            done();
        });
    });
});

describe('avancedb -- db --', function() {
    var testDbName = 'avancedb-test';
    
    it('should get the standard databases', function(done) {
        conn.databases(function(err, dbs) {
            assert.equal(null, err);
            assert.notEqual(null, dbs);
            assert.equal(2, dbs.length);
            assert.equal('_replicator', dbs[0]);
            assert.equal('_users', dbs[1]);
            done();
        });
    });

    it('shouldn\'t find a database', function(done) {
        var db = conn.database(testDbName);
        db.exists(function(err, res) {
            assert.equal(null, err);
            assert.equal(false, res);
            done();
        });
    });

    it('shouldn\'t find database info', function(done) {
        var db = conn.database(testDbName);
        db.info(function(err, res) {
            assert.notEqual(null, err);
            assert.equal('not_found', err.error);
            assert.equal(404, err.headers.status);
            assert.equal(null, res);
            done();
        });
    });
    
    it('shouldn\'t delete a database', function(done) {
        var db = conn.database(testDbName);
        db.destroy(function(err, res) {
            assert.notEqual(null, err);
            assert.equal('not_found', err.error);
            assert.equal(404, err.headers.status);
            assert.equal(null, res);
            done();
        });
    });
    
    it('shouldn\'t create a database: bad name', function(done) {
        var db = conn.database('_' + testDbName);
        db.create(function(err, res) {
            assert.notEqual(null, err);
            assert.equal('illegal_database_name', err.error);
            assert.equal(400, err.headers.status);
            assert.equal(null, res);
            done();
        });
    });

    it('should create a database', function(done) {
        var db = conn.database(testDbName);
        db.create(function(err, res) {
            assert.equal(null, err);
            assert.notEqual(null, res);
            assert.notEqual(res.ok, 'true');
            done();
        });
    });
    
    it('should get the standard databases and the new db', function(done) {
        conn.databases(function(err, dbs) {
            assert.equal(null, err);
            assert.notEqual(false, dbs);
            assert.equal(3, dbs.length);
            assert.equal('_replicator', dbs[0]);
            assert.equal('_users', dbs[1]);
            assert.equal(testDbName, dbs[2]);
            done();
        });
    });
    
    it('create fails: should find an existing database', function(done) {
        var db = conn.database(testDbName);
        db.create(function(err, res) {
            assert.notEqual(null, err);
            assert.equal('file_exists', err.error);
            assert.equal(412, err.headers.status);
            assert.equal(null, res);
            done();
        });
    });

    it('should test for database existence', function(done) {
        var db = conn.database(testDbName);
        db.exists(function(err, res) {
            assert.equal(null, err);
            assert.equal(true, res);
            done();
        });
    });

    it('should get database info', function(done) {
        var db = conn.database(testDbName);
        db.info(function(err, info) {
            assert.equal(null, err);
            assert.notEqual(null, info);
            done();
        });
    });
    
    it('should delete a database', function(done) {
        var db = conn.database(testDbName);
        db.destroy(function(err, res) {
            assert.equal(null, err);
            assert.notEqual(null, res);
            assert.notEqual(res.ok, 'true');
            done();
        });
    });
});

describe('avancedb -- docs --', function() {
    var testDbName = 'avancedb-test';
    var testDocument = { 'lorem' : 'ipsum', pi: 3.14159, sunny: true, the_answer: 42, taxRate: null, fibonnaci: [0, 1, 1, 2, 3, 5, 8, 13 ], child: { 'hello': 'world' }, events: [ null, 1969, 'avance', true, {}, [] ] };
    
    it('should create a database', function(done) {
        var db = conn.database(testDbName);
        db.create(function(err, res) {
            assert.equal(null, err);
            assert.notEqual(null, res);
            assert.notEqual(res.ok, 'true');
            done();
        });
    });
    
    it('get a non-existent document', function(done) {
        var db = conn.database(testDbName);
        db.get('test0', function(err, doc) {
            assert.notEqual(null, err);
            assert.equal('not_found', err.error);
            done();
        });
    });

    it('create a document with an id', function(done) {
        var db = conn.database(testDbName);
        var test0 = _.extend({}, testDocument);
        db.save('test0', test0, function(err, doc) {
            assert.equal(null, err);
            assert.notEqual(null, doc);
            assert.equal('test0', doc._id);
            assert.notEqual(null, doc._rev);
            done();
        });
    });
    
    it('re-use a document id (cradle only)', function(done) {
        var db = conn.database(testDbName);
        var test1 = _.extend({}, testDocument);
        db.save('test0', test1, function(err, doc) {
            assert.equal(null, err);
            assert.notEqual(null, doc);
            assert.equal('test0', doc._id);
            assert.notEqual(null, doc._rev);
            done();
        });
    });
    
    it('get a document', function(done) {
        var db = conn.database(testDbName);
        db.get('test0', function(err, doc) {
            assert.equal(null, err);
            assert.notEqual(null, doc);
            var compDoc = _.extend({}, testDocument);
            compDoc._id = doc._id;
            compDoc._rev = doc._rev;
            assert.deepEqual(doc, compDoc);
            done();
        });
    });
    
    it('delete a document with an id', function(done) {
        var db = conn.database(testDbName);

        db.remove('test0', function(err, res) {
            assert.equal(null, err);
            assert.equal(true, res.ok);
            assert.notEqual(null, res.rev);

            db.get('test0', function(err, doc) {
                assert.notEqual(null, err);
                assert.equal('not_found', err.error);
                assert.equal(404, err.headers.status);
                assert.equal(null, doc);
                done();
            });
        });
    });
    
    it('delete a non-existant document with an id', function(done) {
        var db = conn.database(testDbName);

        db.remove('test0', function(err, res) {
            assert.notEqual(null, err);
            assert.equal('not_found', err.error);
            assert.equal(404, err.headers.status);
            assert.equal(null, res);
            done();
        });
    });
    
    it('create a document with an id, then attempt to update it using the wrong rev', function(done) {
        var db = conn.database(testDbName);
        var test0 = _.extend({}, testDocument);
        db.save('test0', test0, function(err, doc) {
            assert.equal(null, err);
            assert.notEqual(null, doc);
            assert.equal('test0', doc._id);
            assert.notEqual(null, doc._rev);
            
            db.save('test0', { _rev: '0' + doc._rev.slice(1) }, function(err, res) {
                assert.notEqual(null, err);
                assert.equal('conflict', err.error);
                assert.equal(409, err.headers.status);
                done();
            });
        });
    });
    
    it('create a document with an id, then get by that id - 10 times', function(done) {
        var db = conn.database(testDbName);

        var now = new Date().getTime();
        var testPrefix = '' + now + '-';
        var maxDocs = 10, callbackCount = maxDocs;

        for (var i = 0; i < maxDocs; i++) (function() {
            var test = _.extend({}, testDocument);

            // make a unique id
            var id = testPrefix + i;

            // save the doc and then get it back
            db.save(id, test, function(err, doc) {
                assert.equal(null, err);

                db.get(id, function(err, doc) {
                    assert.equal(null, err);
                    assert.notEqual(null, doc);
                    var compDoc = _.extend({}, test);
                    compDoc._id = doc._id;
                    compDoc._rev = doc._rev;
                    assert.deepEqual(doc, compDoc);
                    
                    // countdown the number of callbacks
                    --callbackCount;
                    
                    if (!callbackCount) {
                        done();
                    }
                });
            });
        })();
    });
    
    it('should get document count', function(done) {
        var db = conn.database(testDbName);
        db.info(function(err, info) {
            assert.equal(null, err);
            assert.notEqual(null, info);
            assert.equal(11, info.doc_count);
            done();
        });
    });
    
    it('should delete a database', function(done) {
        var db = conn.database(testDbName);
        db.destroy(function(err, res) {
            assert.equal(null, err);
            assert.notEqual(null, res);
            assert.notEqual(res.ok, 'true');
            done();
        });
    });    
});
