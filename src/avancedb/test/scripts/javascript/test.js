'use strict';

var assert = require('assert');
var cradle = require('cradle');

var host = 'http://localhost';
var port = 15994;
//var port = 5994;
var conn = new cradle.Connection(host, port, { cache: false });

describe('exony-tachosdb -- server info --', function() {
    conn.info(function (err, res) {
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
            assert.equal(null, res);
            done();
        });
    });
    
    it('shouldn\'t delete a database', function(done) {
        var db = conn.database(testDbName);
        db.destroy(function(err, res) {
            assert.notEqual(null, err);
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
