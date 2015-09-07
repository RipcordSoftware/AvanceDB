import couchdb
import unittest
import requests
from copy import deepcopy

host = 'http://localhost'
port = '15994'
url = host + ':' + port
couch = couchdb.Server(url)

    
class ServerInfoTestCase(unittest.TestCase):

    def test_validate_server_signature(self):
        r = requests.get(url + '/')
        self.assertEqual(r.json(), {
                "couchdb":"Welcome",
                "avancedb":"Welcome",
                "uuid":"a2db86472466bcd02e84ac05a6c86185",
                "version":"1.6.1",
                "vendor":{
                    "version":"0.0.1", "name":"Ripcord Software"
                }
            })

class FutonTestCase(unittest.TestCase):

    def test_redirect(self):
        r = requests.get(url + '/_utils', allow_redirects=False)
        self.assertEqual(r.status_code, 307)
        self.assertEqual(r.headers['location'], '/_utils/index.html')
    
    def test_get_html(self):
        r = requests.get(url + '/_utils')
        self.assertEqual(r.status_code, 200)
        self.assertEqual(r.headers['content-type'], 'text/html')
    
    def test_get_favicon(self):
        r = requests.get(url + '/_utils/favicon.ico')
        self.assertEqual(r.status_code, 200)
        self.assertEqual(r.headers['content-type'], 'image/ico')
    
    def test_get_logo(self):
        r = requests.get(url + '/_utils/image/logo.png')
        self.assertEqual(r.status_code, 200)
        self.assertEqual(r.headers['content-type'], 'image/png')
    
    def test_not_valid_response(self):
        r = requests.get(url + '/_utils/nothing_to_see_here')
        self.assertEqual(r.status_code, 404)
        self.assertEqual(r.headers['content-length'], '0')

class ConfigTestCase(unittest.TestCase):

    def test_query_servers(self):
        r = requests.get(url + '/_config/query_servers')
        self.assertEqual(r.status_code, 200)
        self.assertEqual(r.headers['content-type'], 'application/json')
        self.assertNotEqual(r.json()[u'javascript'], None)

    def test_native_query_servers(self):
        r = requests.get(url + '/_config/native_query_servers')
        self.assertEqual(r.status_code, 200)
        self.assertEqual(r.headers['content-type'], 'application/json')
        self.assertEqual(r.json(), {})
    
class TasksTestCase(unittest.TestCase):

    def test_no_tasks(self):
        tasks = couch.tasks();
        self.assertNotEqual(tasks, None)
        self.assertEqual(len(tasks), 0)

class SessionTestCase(unittest.TestCase):

    def test_session_response(self):
        r = requests.get(url + '/_session')
        self.assertEqual(r.status_code, 200)
        self.assertEqual(r.headers['content-type'], 'application/json')
        json = r.json()
        self.assertNotEqual(json[u'ok'], None)
        self.assertNotEqual(json[u'userCtx'], None)
        
class UuidTestCase(unittest.TestCase):

    def test_get_one_uuid(self):
        uuids = couch.uuids(1)
        self.assertEqual(len(uuids), 1)

    def test_get_zero_uuids_return_zero(self):
        uuids = couch.uuids(0)
        self.assertEqual(len(uuids), 0)

    def test_get_minus_one_uuid(self):
        uuids = couch.uuids(-1)
        self.assertEqual(len(uuids), 0)

    def test_get_one_hundred_uuids(self):
        uuids = couch.uuids(100)
        self.assertEqual(len(uuids), 100)

    def test_get_one_thousand_uuids(self):
        uuids = couch.uuids(1000)
        self.assertEqual(len(uuids), 1000)

    def test_get_ten_thousand_uuids(self):
        with self.assertRaises(couchdb.ServerError) as se:
            couch.uuids(10000)

        self.assertEqual(se.exception[0][0], 403)
        self.assertEqual(se.exception[0][1][0], u'forbidden')
        
class DbTestCase(unittest.TestCase):
    test_db_name = 'avancedb-test'
    
    def tearDown(self):
        try :
            couch.delete(self.test_db_name)
        except Exception:
            pass

    def test_get_standard_databases(self):
        self.assertEqual(len(couch), 2)
        self.assertIsNotNone(couch['_replicator'])
        self.assertIsNotNone(couch['_users'])

    def test_shouldnt_find_db(self):
        self.assertFalse(self.test_db_name in couch)

    def test_shouldnt_find_db_info(self):
        r = requests.get(url + '/' + self.test_db_name + '/_info')
        self.assertEqual(r.status_code, 404)
        self.assertEqual(r.json()[u'error'], u'not_found')

    def test_shouldnt_delete_db(self):
        r = requests.delete(url + '/' + self.test_db_name)
        self.assertEqual(r.status_code, 404)
        self.assertEqual(r.json()[u'error'], u'not_found')

    def test_shouldnt_delete_replicator_db(self):
        r = requests.delete(url + '/_replicator')
        self.assertEqual(r.status_code, 400)
        self.assertEqual(r.json()[u'error'], u'illegal_database_name')

    def test_shouldnt_delete_users_db(self):
        r = requests.delete(url + '/_users')
        self.assertEqual(r.status_code, 400)
        self.assertEqual(r.json()[u'error'], u'illegal_database_name')

    def test_shouldnt_create_database_bad_name(self):
        with self.assertRaises(couchdb.ServerError) as se:
            couch.create('_' + self.test_db_name)

        self.assertEqual(se.exception[0][0], 400)
        self.assertEqual(se.exception[0][1][0], 'illegal_database_name')

    def test_should_create_database(self):
        try :
            couch.create(self.test_db_name)
        except Exception:
            self.fail('Failed to create database')

    def test_should_get_created_database_and_system_databases(self):
        couch.create(self.test_db_name)
        self.assertEqual(len(couch), 3)
        self.assertIsNotNone(couch['_replicator'])
        self.assertIsNotNone(couch['_users'])
        self.assertIsNotNone(couch[self.test_db_name])

    def test_shouldnt_create_an_existing_database(self):
        couch.create(self.test_db_name)
        with self.assertRaises(couchdb.PreconditionFailed) as se:
            couch.create(self.test_db_name)
        
        self.assertEqual(se.exception[0][0], 'file_exists')

    def test_should_delete_a_database(self):
        couch.create(self.test_db_name)
        couch.delete(self.test_db_name)
        r = requests.get(url + '/' + self.test_db_name + '/_info')
        self.assertEqual(r.status_code, 404)
        self.assertEqual(r.json()[u'error'], u'not_found')

class DocsTestCase(unittest.TestCase):
    test_db_name = 'avancedb-test'
    testDocument = { 'lorem' : 'ipsum', 'pi': 3.14159, 'sunny': True, 'free_lunch': False, 'the_answer': 42,
    'taxRate': None, 'fibonnaci': [0, 1, 1, 2, 3, 5, 8, 13 ], 'child': { 'hello': 'world' },
    'events': [ None, 1969, 'avance', True, {}, [] ], 'data': '0123456789'}

    for i in xrange(0, 8):
        testDocument['data'] += testDocument['data']
        
    def setUp(self):
            couch.create(self.test_db_name)

    def tearDown(self):
        try :
            couch.delete(self.test_db_name)
        except Exception:
            pass

    def test_should_not_get_non_existent_document(self):
        doc = couch[self.test_db_name].get('test0')
        self.assertIsNone(doc)
        
    def test_should_create_a_document_with_id(self):
        test_doc = self.testDocument
        test_doc['_id'] = 'test0'
        doc = couch[self.test_db_name].save(test_doc)
        self.assertEqual(doc[0], u'test0')
        self.assertIsNotNone(doc[1])
        
    def test_should_not_create_document_with_bad_rev(self):
        test_doc = deepcopy(self.testDocument)
        test_doc['_id'] = 'test1'
        test_doc['_rev'] = 'abcdef'
        with self.assertRaises(couchdb.ServerError) as se:
            couch[self.test_db_name].save(test_doc)

        self.assertEqual(se.exception[0][0], 400)
        self.assertEqual(se.exception[0][1][0], 'bad_request')
        
    def test_should_not_update_document_with_bad_rev(self):
        test_doc = deepcopy(self.testDocument)
        test_doc['_id'] = 'test1'
        couch[self.test_db_name].save(test_doc)
        test_doc['_rev'] = 'abcdef'
        with self.assertRaises(couchdb.ResourceConflict) as se:
            couch[self.test_db_name].save(test_doc)

        self.assertEqual(se.exception[0][0], 'conflict')
        
    def test_get_a_document(self):
        test_doc = deepcopy(self.testDocument)
        test_doc['_id'] = 'test0'
        couch[self.test_db_name].save(test_doc)
        doc = couch[self.test_db_name].get('test0')
        test_doc['_rev'] = doc['_rev']
        self.assertEqual(doc, test_doc)
        
    #fails
    def test_get_a_document_all_docs(self):
        test_doc = deepcopy(self.testDocument)
        test_doc['_id'] = 'test0'
        couch[self.test_db_name].save(test_doc)
        docs = couch[self.test_db_name].view('_all_docs')
        self.assertEqual(len(docs.rows), 1)
        self.assertIsNotNone(docs.rows[0].id)
        self.assertEqual(docs.rows[0].id, docs.rows[0].key)

        self.assertIsNotNone(docs.rows[0].value)
        self.assertIsNotNone(docs.rows[0].value['rev'])

    def test_should_not_delete_doc_with_id_but_bad_rev(self):
        test_doc = deepcopy(self.testDocument)
        test_doc['_id'] = 'test0'
        couch[self.test_db_name].save(test_doc)
        test_doc['_rev'] = 'abcdef'
        with self.assertRaises(couchdb.ServerError) as se:
            couch[self.test_db_name].delete(test_doc)

        self.assertEqual(se.exception[0][0], 400)
        self.assertEqual(se.exception[0][1][0], 'bad_request')
        
    def test_should_delete_a_doc_by_id(self):
        test_doc = deepcopy(self.testDocument)
        test_doc['_id'] = 'test0'
        
        couch[self.test_db_name].save(test_doc)
        couch[self.test_db_name].delete(test_doc)
        doc = couch[self.test_db_name].get('test0')
        self.assertIsNone(doc)

    def test_should_not_delete_doc_that_doesnt_exist(self):
        test_doc = deepcopy(self.testDocument)

        with self.assertRaises(couchdb.ResourceNotFound) as se:
            couch[self.test_db_name].delete(test_doc)

        self.assertEqual(se.exception[0][0], u'not_found')
        
    def test_fail_to_update_doc_with_wrong_rev(self):
        test_doc = deepcopy(self.testDocument)
        test_doc['_id'] = 'test0'
        couch[self.test_db_name].save(test_doc)
        doc = couch[self.test_db_name].get('test0')
        self.assertIsNotNone(doc['_rev'])
        self.assertEqual(doc['_id'], 'test0')
        doc['_rev'] = '00' + doc['_rev']
        with self.assertRaises(couchdb.ResourceConflict) as se:
            couch[self.test_db_name].save(doc)
        
        self.assertEqual(se.exception[0][0], 'conflict')
        

        
        
    
if __name__ == "__main__":
    unittest.main() # run all tests
