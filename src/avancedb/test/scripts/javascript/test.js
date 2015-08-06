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

describe('avancedb -- session --', function() {
    it('should get the default session response', function(done) {
        http.get(url + '/_session', function(res) {
            assert.equal(200, res.statusCode);
            assert.equal('application/json', res.headers['content-type']);
            res.on('data', function(chunk) {
                var session = JSON.parse(chunk);
                assert.notEqual(null, session.ok);
                assert.notEqual(null, session.userCtx);
                done();
            });
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
    
    it('shouldn\'t delete the _replicator database', function(done) {
        var db = conn.database('_replicator');
        db.destroy(function(err, res) {
            assert.notEqual(null, err);
            assert.equal('illegal_database_name', err.error);
            assert.equal(400, err.headers.status);
            assert.equal(null, res);
            done();
        });
    });
    
    it('shouldn\'t delete the _users database', function(done) {
        var db = conn.database('_users');
        db.destroy(function(err, res) {
            assert.notEqual(null, err);
            assert.equal('illegal_database_name', err.error);
            assert.equal(400, err.headers.status);
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
    var testDocument = { 'lorem' : 'ipsum', pi: 3.14159, sunny: true, free_lunch: false, the_answer: 42, 
        taxRate: null, fibonnaci: [0, 1, 1, 2, 3, 5, 8, 13 ], child: { 'hello': 'world' }, 
        events: [ null, 1969, 'avance', true, {}, [] ], //minNUm: Number.MIN_VALUE , maxNum: Number.MAX_VALUE,
        data: '0123456789' };
        
    for (var i = 0; i < 8; ++i) {
        testDocument.data += testDocument.data;
    }
    
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
    
    it('get a document via _all_docs', function(done) {
        var db = conn.database(testDbName);
        db.all(function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(1, docs.length);
            assert.notEqual(null, docs[0].id);
            assert.equal(docs[0].id, docs[0].key);
            assert.notEqual(null, docs[0].value);
            assert.notEqual(null, docs[0].value.rev);
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
    
    it('delete a non-existent document with an id', function(done) {
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
    
    it('create a document with an id, then get by that id - 100 times - ascending', function(done) {
        var db = conn.database(testDbName);

        var now = new Date().getTime();
        var testPrefix = '' + now + '-';
        var maxDocs = 100, callbackCount = maxDocs;

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
    
    it('create a document with an id, then get by that id - 100 times - descending', function(done) {
        var db = conn.database(testDbName);

        var now = new Date().getTime();
        var testPrefix = '' + now + '-';
        var maxDocs = 100, callbackCount = maxDocs;

        for (var i = 0; i < maxDocs; i++) (function() {
            var test = _.extend({}, testDocument);

            // make a unique id
            var id = testPrefix + (maxDocs - i);

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
    
    it('create a document with an id, then get by that id - 100 times - shuffled', function(done) {
        var db = conn.database(testDbName);

        var now = new Date().getTime();
        var testPrefix = '' + now + '-';
        var maxDocs = 100, callbackCount = maxDocs;
        var indexes = [];
        
        for (var i = 0; i < maxDocs; i++) {
            indexes[i] = i;
        }
        
        indexes = _.shuffle(indexes);

        for (var i = 0; i < maxDocs; i++) (function() {
            var test = _.extend({}, testDocument);

            // make a unique id
            var id = testPrefix + indexes[i];

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
    
    it('create a document with an id, then get by that id - 100 times - uuid', function(done) {
        conn.uuids(100, function(err, uuids) {
            assert.equal(null, err);
            assert.notEqual(null, uuids);
            
            var maxDocs = uuids.length, callbackCount = maxDocs;
            var db = conn.database(testDbName);

            for (var i = 0; i < maxDocs; i++) (function() {
                var test = _.extend({}, testDocument);

                // a unique id
                var id = uuids[i];

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
    });
    
    it('get documents via _all_docs', function(done) {
        var db = conn.database(testDbName);
        db.all(function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(401, docs.length);
            for (var i = 0; i < docs.length; ++i) {
                assert.notEqual(null, docs[i].id);
                assert.equal(docs[i].id, docs[i].key);
                assert.notEqual(null, docs[i].value);
                assert.notEqual(null, docs[i].value.rev);
            }
            done();
        });
    });
    
    it('should get document count', function(done) {
        var db = conn.database(testDbName);
        db.info(function(err, info) {
            assert.equal(null, err);
            assert.notEqual(null, info);
            assert.equal(401, info.doc_count);
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

describe('avancedb -- _all_docs --', function() {
    var testDbName = 'avancedb-all_docs-test';
    var db = conn.database(testDbName);

    // create sequential ids
    var ids = [];
    for (var i = 0; i < 15; i++) {
        ids[i] = i.toString();
    }

    // sort the ids to match couchdb
    ids.sort();

    // reverse the sorted ids
    var revIds = ids.slice(0);
    revIds.reverse();

    it('shouldn\'t find a database', function(done) {
        db.exists(function(err, res) {
            assert.equal(null, err);
            assert.equal(false, res);
            done();
        });
    });

    it('should create a database', function(done) {
        db.create(function(err, res) {
            assert.equal(null, err);
            assert.notEqual(null, res);
            assert.notEqual(res.ok, 'true');
            done();
        });
    });

    it('create a sequence of documents', function(done) {
        var callbackCount = ids.length;
        for (var i = 0; i < ids.length; i++) {
            (function() {
                var _id = i.toString();
                db.save({ _id: _id }, function(err, doc) {
                    assert.equal(null, err);
                    assert.notEqual(null, doc);
                    assert.notEqual(null, doc._id);
                    assert.equal(_id, doc._id);

                    // countdown the callbacks
                    --callbackCount;
                    !callbackCount && done();
                });
            })();
        }
    });
    
    it('check document update sequence', function(done) {
        db.all({update_seq:true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(ids.length, docs.length);
            assert.equal(ids.length, docs.update_seq);        

            done();
        });
    });
    
    it('check document update sequence with malformed parameter', function(done) {
        db.all({update_seq:'tru'}, function(err, docs) {
            assert.notEqual(null, err);
            assert.equal('query_parse_error', err.error);

            done();
        });
    });
    
    it('check document update sequence with keys', function(done) {
        db.all({keys: ids, update_seq:true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(ids.length, docs.length);
            assert.equal(ids.length, docs.update_seq);        

            done();
        });
    });
    
    it('check document update sequence with keys and malformed parameter', function(done) {
        db.all({keys: ids, update_seq:'tru'}, function(err, docs) {
            assert.notEqual(null, err);
            assert.equal('query_parse_error', err.error);

            done();
        });
    });
    
    it('check document count and order; with include docs', function(done) {
        db.all({include_docs:true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(ids.length, docs.length);

            for (var i = 0; i < ids.length; i++) {
                assert.equal(ids[i], docs[i].id);
                assert.equal(ids[i], docs[i].value.doc._id);
            }

            done();
        });
    });

    it('check limit', function(done) {
        db.all({limit: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(1, docs.length);
            assert.equal(ids[0], docs[0].id);
            done();
        });
    });
    
    it('check negative limit', function(done) {
        db.all({limit: -1}, function(err, docs) {
            assert.notEqual(null, err);
            assert.equal('query_parse_error', err.error);
            done();
        });
    });
    
    it('check negative skip', function(done) {
        db.all({skip: -1}, function(err, docs) {
            assert.notEqual(null, err);
            assert.equal('query_parse_error', err.error);
            done();
        });
    });

    it('check limit=1 and skip=1', function(done) {
        db.all({limit: 1, skip: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(1, docs.length);
            assert.equal(ids[1], docs[0].id);
            done();
        });
    });

    it('check limit=2 and skip=1', function(done) {
        db.all({limit: 2, skip: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(2, docs.length);
            assert.equal(ids[1], docs[0].id);
            assert.equal(ids[2], docs[1].id);
            done();
        });
    });
    
    it('check limit=5 and skip=2', function(done) {
        db.all({limit: 5, skip: 2}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(5, docs.length);
            assert.equal(ids[2], docs[0].id);
            assert.equal(ids[3], docs[1].id);
            assert.equal(ids[4], docs[2].id);
            assert.equal(ids[5], docs[3].id);
            assert.equal(ids[6], docs[4].id);
            done();
        });
    });

    it('check descending', function(done) {
        db.all({descending: true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(ids.length, docs.length);

            for (var i = 0; i < ids.length; i++) {
                assert.equal(revIds[i], docs[i].id);
            }

            done();
        });
    });

    it('check descending, limit=1', function(done) {
        db.all({descending: true, limit: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(1, docs.length);
            assert.equal(revIds[0], docs[0].id);
            done();
        });
    });
    
    it('check descending, limit=2', function(done) {
        db.all({descending: true, limit: 2}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(2, docs.length);
            assert.equal(revIds[0], docs[0].id);
            assert.equal(revIds[1], docs[1].id);
            done();
        });
    });

    it('check descending, limit=1, skip=1', function(done) {
        db.all({descending: true, limit: 1, skip: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(1, docs.length);
            assert.equal(revIds[1], docs[0].id);
            done();
        });
    });
    
    it('check descending, limit=2, skip=4', function(done) {
        db.all({descending: true, limit: 2, skip: 4}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(2, docs.length);
            assert.equal(revIds[4], docs[0].id);
            assert.equal(revIds[5], docs[1].id);
            done();
        });
    });
    
    it('check key', function(done) {
        db.all({key: '3'}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsIndex = ids.indexOf('3');
            assert.equal(1, docs.length);
            assert.equal(ids[idsIndex], docs[0].id);
            
            done();
        });
    });
    
    it('check key, limit=1, skip=0', function(done) {
        db.all({key: '3', limit: 1, skip: 0}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsIndex = ids.indexOf('3');
            assert.equal(1, docs.length);
            assert.equal(ids[idsIndex], docs[0].id);
            
            done();
        });
    });
    
    it('check key, limit=0, skip=1', function(done) {
        db.all({key: '3', limit: 0, skip: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            assert.equal(0, docs.length);
            
            done();
        });
    });
    
    it('check missed key', function(done) {
        db.all({key: 'x'}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            assert.equal(0, docs.length);
            
            done();
        });
    });

    it('check startkey', function(done) {
        db.all({startkey: '3'}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsIndex = ids.indexOf('3');
            assert.equal(ids.length - idsIndex, docs.length);

            for (var i = 0; i < docs.length; i++) {
                assert.equal(ids[idsIndex], docs[i].id);
                idsIndex++;
            }

            done();
        });
    });

    it('check startkey, descending', function(done) {
        db.all({startkey: '3', descending: true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsIndex = revIds.indexOf('3');
            assert.equal(revIds.length - idsIndex, docs.length);

            for (var i = 0; i < docs.length; i++) {
                assert.equal(revIds[idsIndex], docs[i].id);
                idsIndex++;
            }

            done();
        });
    });

    it('check startkey, descending, limit=1', function(done) {
        db.all({startkey: '3', descending: true, limit: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            assert.equal(1, docs.length);
            assert.equal('3', docs[0].id);
            done();
        });
    });

    it('check startkey, descending, limit=1, skip=1', function(done) {
        db.all({startkey: '3', descending: true, limit: 1, skip: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            assert.equal(1, docs.length);

            var idsIndex = revIds.indexOf('3');
            assert.equal(revIds[idsIndex + 1], docs[0].id);
            done();
        });
    });

    it('check endkey', function(done) {
        db.all({endkey: '3'}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsIndex = ids.indexOf('3');
            assert.equal(idsIndex + 1, docs.length);

            for (var i = 0; i < docs.length; i++) {
                assert.equal(ids[i], docs[i].id);
            }

            done();
        });
    });

    it('check endkey, no inclusive end', function(done) {
        db.all({endkey: '3', inclusive_end: false}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsIndex = ids.indexOf('3');
            assert.equal(idsIndex, docs.length);

            for (var i = 0; i < docs.length; i++) {
                assert.equal(ids[i], docs[i].id);
            }

            done();
        });
    });

    it('check endkey, limit=1', function(done) {
        db.all({endkey: '3', limit: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            assert.equal(1, docs.length);
            assert.equal(ids[0], docs[0].id);

            done();
        });
    });

    it('check endkey, limit=3', function(done) {
        db.all({endkey: '3', limit: 3}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            assert.equal(3, docs.length);

            for (var i = 0; i < docs.length; i++) {
                assert.equal(ids[i], docs[i].id);
            }

            done();
        });
    });

    it('check endkey, limit=3, skip=1', function(done) {
        db.all({endkey: '3', limit: 3, skip: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            assert.equal(3, docs.length);

            for (var i = 0; i < docs.length; i++) {
                assert.equal(ids[i + 1], docs[i].id);
            }

            done();
        });
    });

    it('check endkey, descending', function(done) {
        db.all({endkey: '3', descending: true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsIndex = revIds.indexOf('3');
            assert.equal(idsIndex + 1, docs.length);

            for (var i = 0; i < docs.length; i++) {
                assert.equal(revIds[i], docs[i].id);
            }

            done();
        });
    });

    it('check endkey, descending, no inclusive end', function(done) {
        db.all({endkey: '3', descending: true, inclusive_end: false}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsIndex = revIds.indexOf('3');
            assert.equal(idsIndex, docs.length);

            for (var i = 0; i < docs.length; i++) {
                assert.equal(revIds[i], docs[i].id);
            }

            done();
        });
    });

    it('check endkey, descending, limit=1', function(done) {
        db.all({endkey: '3', descending: true, limit: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            assert.equal(1, docs.length);
            assert.equal(revIds[0], docs[0].id);

            done();
        });
    });

    it('check endkey, descending, limit=1, skip=1', function(done) {
        db.all({endkey: '3', descending: true, limit: 1, skip: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            assert.equal(1, docs.length);
            assert.equal(revIds[1], docs[0].id);

            done();
        });
    });

    it('check endkey, descending, limit=20, skip=2', function(done) {
        db.all({endkey: '3', descending: true, limit: 20, skip: 2}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsIndex = revIds.indexOf('3');
            assert.equal(idsIndex - 1, docs.length);
            for (var i = 0; i < docs.length; i++) {
                assert.equal(revIds[i + 2], docs[i].id);
            }

            done();
        });
    });

    it('check startkey, endkey', function(done) {
        db.all({startkey: '0', endkey:'3'}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsIndex = ids.indexOf('3');
            assert.equal(idsIndex + 1, docs.length);
            for (var i = 0; i < docs.length; i++) {
                assert.equal(ids[i], docs[i].id);
            }

            done();
        });
    });

    it('check startkey, endkey, not inclusive_end', function(done) {
        db.all({startkey: '0', endkey:'3', inclusive_end: false}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsIndex = ids.indexOf('3');
            assert.equal(idsIndex, docs.length);
            for (var i = 0; i < docs.length; i++) {
                assert.equal(ids[i], docs[i].id);
            }

            done();
        });
    });

    it('check startkey, endkey, limit=1', function(done) {
        db.all({startkey: '0', endkey:'3', limit: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(1, docs.length);
            assert.equal(ids[0], docs[0].id);
            done();
        });
    });

    it('check startkey, endkey, limit=1, skip=1', function(done) {
        db.all({startkey: '0', endkey:'3', limit: 1, skip: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(1, docs.length);
            assert.equal(ids[1], docs[0].id);
            done();
        });
    });

    it('check startkey, endkey', function(done) {
        db.all({startkey: '1', endkey:'3'}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsStartIndex = ids.indexOf('1');
            var idsEndIndex = ids.indexOf('3');
            assert.equal(idsEndIndex - idsStartIndex + 1, docs.length);
            for (var i = 0; i < docs.length; i++) {
                assert.equal(ids[idsStartIndex + i], docs[i].id);
            }
            done();
        });
    });

    it('check startkey, endkey, descending', function(done) {
        db.all({startkey: '0', endkey:'3', descending: true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(0, docs.length);
            done();
        });
    });

    it('check reversed startkey, endkey, descending', function(done) {
        db.all({startkey: '3', endkey:'0', descending: true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsStartIndex = revIds.indexOf('3');
            var idsEndIndex = revIds.indexOf('0');
            assert.equal(idsEndIndex - idsStartIndex + 1, docs.length);
            for (var i = 0; i < docs.length; i++) {
                assert.equal(revIds[idsStartIndex + i], docs[i].id);
            }

            done();
        });
    });

    it('check reversed startkey, endkey, descending', function(done) {
        db.all({startkey: '3', endkey:'1', descending: true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsStartIndex = revIds.indexOf('3');
            var idsEndIndex = revIds.indexOf('1');
            assert.equal(idsEndIndex - idsStartIndex + 1, docs.length);
            for (var i = 0; i < docs.length; i++) {
                assert.equal(revIds[idsStartIndex + i], docs[i].id);
            }

            done();
        });
    });

    it('check reversed startkey, endkey, descending, limit=1', function(done) {
        db.all({startkey: '3', endkey:'1', descending: true, limit: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsStartIndex = revIds.indexOf('3');
            assert.equal(revIds[idsStartIndex], docs[0].id);

            done();
        });
    });

    it('check reversed startkey, endkey, descending, limit=1, skip=1', function(done) {
        db.all({startkey: '3', endkey:'1', descending: true, limit: 1, skip: 1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);

            var idsStartIndex = revIds.indexOf('3');
            assert.equal(revIds[idsStartIndex + 1], docs[0].id);

            done();
        });
    });
    
    it('check with keys', function(done) {
        var keys = ids.slice(0, 4);
        db.all({keys: keys}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(keys.length, docs.length);

            for (var i = 0; i < keys.length; i++) {
                assert.equal(keys[i], docs[i].id);
            }

            done();
        });
    });
    
    it('check with keys with include_docs', function(done) {
        var keys = ids.slice(0, 4);
        db.all({keys: keys, include_docs: true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(keys.length, docs.length);

            for (var i = 0; i < keys.length; i++) {
                assert.equal(keys[i], docs[i].id);
                assert.equal(keys[i], docs[i].value.doc._id);
            }

            done();
        });
    });
    
    it('check with keys with malformed include_docs', function(done) {
        var keys = ids.slice(0, 4);
        db.all({keys: keys, include_docs: 'tru'}, function(err, docs) {
            assert.notEqual(null, err);
            assert.equal('query_parse_error', err.error);

            done();
        });
    });
    
    it('check with keys, descending', function(done) {
        var keys = ids.slice(0, 4);
        db.all({keys: keys, descending: true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(keys.length, docs.length);

            for (var i = 0; i < keys.length; i++) {
                assert.equal(keys[i], docs[keys.length - 1 - i].id);
            }

            done();
        });
    });
    
    it('check with keys and limit 10', function(done) {
        var keys = ids.slice(0, 4);
        db.all({keys: keys, limit:10}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(keys.length, docs.length);

            for (var i = 0; i < keys.length; i++) {
                assert.equal(keys[i], docs[i].id);
            }

            done();
        });
    });
    
    it('check all with keys and limit 1', function(done) {
        var keys = ids.slice(1, 4);
        db.all({keys: keys, limit:1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(1, docs.length);
            assert.equal(keys[0], docs[0].id);

            done();
        });
    });
    
    it('check all with keys, limit 1 and skip 1', function(done) {
        var keys = ids.slice(1, 4);
        db.all({keys: keys, limit:1, skip:1}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(1, docs.length);
            assert.equal(keys[1], docs[0].id);

            done();
        });
    });
    
    it('check all with keys, limit 1 and skip 100', function(done) {
        var keys = ids.slice(1, 4);
        db.all({keys: keys, limit:1, skip:100}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(0, docs.length);

            done();
        });
    });

    it('check with all keys', function(done) {
        db.all({keys: ids}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(ids.length, docs.length);

            for (var i = 0; i < ids.length; i++) {
                assert.equal(ids[i], docs[i].id);
            }

            done();
        });
    });
    
    it('check with all keys, negative skip', function(done) {
        db.all({keys: ids, skip: -1}, function(err, docs) {
            assert.notEqual(null, err);
            assert.equal('query_parse_error', err.error);

            done();
        });
    });
    
    it('check with all keys, negative limit', function(done) {
        db.all({keys: ids, limit: -1}, function(err, docs) {
            assert.notEqual(null, err);
            assert.equal('query_parse_error', err.error);

            done();
        });
    });
    
    it('check with all keys, descending', function(done) {
        db.all({keys: ids, descending: true}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(ids.length, docs.length);

            for (var i = 0; i < ids.length; i++) {
                assert.equal(ids[i], docs[ids.length - 1 - i].id);
            }

            done();
        });
    });
    
    it('check with all keys, malformed descending', function(done) {
        db.all({keys: ids, descending: 'tru'}, function(err, docs) {
            assert.notEqual(null, err);
            assert.equal('query_parse_error', err.error);

            done();
        });
    });
    
    it('check with all keys, with modified first missing doc', function(done) {
        var keys = ids.slice(0);
        keys[0] = 'abc';
        db.all({keys: keys}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(keys.length, docs.length);

            for (var i = 0; i < keys.length; i++) {
                assert.equal(keys[i], docs[i].key);
            }       
            
            assert.equal("not_found", docs[0].error);

            done();
        });
    });    
    
    it('check with all keys, with added last missing doc', function(done) {
        var keys = ids.slice(0);
        keys.push('abc');
        db.all({keys: keys}, function(err, docs) {
            assert.equal(null, err);
            assert.notEqual(null, docs);
            assert.equal(keys.length, docs.length);

            for (var i = 0; i < keys.length; i++) {
                assert.equal(keys[i], docs[i].key);
            }       
            
            assert.equal("not_found", docs[keys.length - 1].error);

            done();
        });
    });

    it('delete the database', function(done) {
        db.destroy(function(err, res) {
            assert.equal(null, err);
            assert.notEqual(null, res);
            done();
        });
    });
});
