'use strict';

var assert = require('assert');
var cradle = require('cradle');
var _ = require('underscore');

var host = 'http://localhost';
var port = 15994;
//var port = 5994;
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

describe('avancedb -- db --', function() {
    var testDbName = 'avancedb-test';

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

    it('should create a database', function(done) {
        var db = conn.database(testDbName);
        db.create(function(err, res) {
            assert.equal(null, err);
            assert.notEqual(null, res);
            assert.notEqual(res.ok, 'true');
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
    var testDocument = { 'lorem' : 'ipsum' };
    
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
            assert.equal('test0', doc._id);
            assert.equal(testDocument.hello, doc.hello);
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
                    assert.equal(id, doc._id);
                    assert.equal(testDocument.hello, doc.hello);
                    
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
