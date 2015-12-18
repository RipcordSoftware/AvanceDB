import couchdb
from couchdb.design import ViewDefinition
import unittest
import requests
from copy import deepcopy
from random import shuffle

host = 'http://localhost'
port = '15996'
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
                    "version":"0.2.0", "name":"Ripcord Software"
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

    def test_create_doc_with_id_and_get_100_times(self):
        num_docs = 100
        for i in xrange(0, num_docs):
            doc_id = 'test' + str(i)
            test_doc = deepcopy(self.testDocument)
            test_doc['_id'] = doc_id
            couch[self.test_db_name].save(test_doc)
            doc = couch[self.test_db_name].get(doc_id)
            self.assertEqual(doc, test_doc)
            self.assertIsNotNone(doc['_rev'])

    def test_create_doc_with_id_and_get_100_times_descending(self):
        num_docs = 100
        for i in xrange(0, num_docs):
            doc_id = 'test' + str(num_docs - i)
            test_doc = deepcopy(self.testDocument)
            test_doc['_id'] = doc_id
            couch[self.test_db_name].save(test_doc)
            doc = couch[self.test_db_name].get(doc_id)
            self.assertEqual(doc, test_doc)
            self.assertIsNotNone(doc['_rev'])

    def test_create_doc_with_id_and_get_100_times_shuffled(self):
        num_docs = 100
        id_suffixes = range(0, 100)
        shuffle(id_suffixes)

        for i in xrange(0, num_docs):
            doc_id = 'test' + str(id_suffixes[i])
            test_doc = deepcopy(self.testDocument)
            test_doc['_id'] = doc_id
            couch[self.test_db_name].save(test_doc)
            doc = couch[self.test_db_name].get(doc_id)
            self.assertEqual(doc, test_doc)
            self.assertIsNotNone(doc['_rev'])

    def test_create_doc_with_id_and_get_100_times_uuids(self):
        num_docs = 100
        uuids = couch.uuids(num_docs)
        for i in xrange(0, num_docs):
            doc_id = 'test' + str(uuids[i])
            test_doc = deepcopy(self.testDocument)
            test_doc['_id'] = doc_id
            couch[self.test_db_name].save(test_doc)
            doc = couch[self.test_db_name].get(doc_id)
            self.assertEqual(doc, test_doc)
            self.assertIsNotNone(doc['_rev'])


class DesignsTestCase(unittest.TestCase):
    test_db_name = 'avancedb-test'
        
    def setUp(self):
        couch.create(self.test_db_name)
        self.testDesign = ViewDefinition('test0', 'test', '''\function(doc) { emit(null, doc); }''')
        self.testDesign2 = ViewDefinition('test1', 'test', '''function(doc) {
           emit('hello world', 'I\nam\na\nmulti-line\nand\ttabbed\tstring');
        }''')

    def tearDown(self):
        try :
            couch.delete(self.test_db_name)
        except Exception:
            pass

    def test_get_a_non_existent_design(self):
        doc = couch[self.test_db_name].get('_design/testX')
        self.assertIsNone(doc)

    def test_create_and_get_design_with_id(self):
        test_doc = deepcopy(self.testDesign)
        test_doc.sync(couch[self.test_db_name])
        doc = couch[self.test_db_name].get('_design/test0')
        self.assertEqual(doc['_id'], u'_design/test0')
        self.assertIsNotNone(doc['_rev'])

    def test_create_and_get_design_with_used_id(self):
        test_doc = deepcopy(self.testDesign)
        test_doc.sync(couch[self.test_db_name])
        doc = couch[self.test_db_name].get('_design/test0')
        self.assertEqual(doc['_id'], u'_design/test0')
        self.assertIsNotNone(doc['_rev'])

    def test_get_design_via_all_docs(self):
        test_doc = deepcopy(self.testDesign)
        test_doc.sync(couch[self.test_db_name])

        docs = couch[self.test_db_name].view('_all_docs')
        self.assertEqual(len(docs.rows), 1)
        self.assertEqual(docs.rows[0].id, u'_design/test0')
        self.assertEqual(docs.rows[0].id, docs.rows[0].key)

        self.assertIsNotNone(docs.rows[0].value)
        self.assertIsNotNone(docs.rows[0].value['rev'])
        
    def test_should_delete_a_design(self):
        test_doc = deepcopy(self.testDesign)
        test_doc.sync(couch[self.test_db_name])
        
        doc = test_doc.get_doc(couch[self.test_db_name])
        couch[self.test_db_name].delete(doc)
        doc = couch[self.test_db_name].get('_design/test0')
        self.assertIsNone(doc)

    def test_design_included_in_doc_count(self):
        test_doc = deepcopy(self.testDesign)
        test_doc.sync(couch[self.test_db_name])
        test_doc2 = deepcopy(self.testDesign2)
        test_doc2.sync(couch[self.test_db_name])

        info = couch[self.test_db_name].info()
        self.assertEqual(2, info['doc_count'])
  
class AllDocsTestCase(unittest.TestCase):
    test_db_name = 'avancedb-test'
    ids = []

    def setUp(self):
        couch.create(self.test_db_name)
        ids = [];
        for i in xrange(15):
            _id = str(i)
            ids.append(_id)
        ids.sort()
        self.ids = ids
        for i in xrange(len(ids)):
            _id = str(i)
            couch[self.test_db_name][_id] = {}


    def tearDown(self):
        try :
            couch.delete(self.test_db_name)
        except Exception:
            pass

    def test_check_document_update_sequence(self):
        docs = couch[self.test_db_name].view('_all_docs', update_seq = True)
        self.assertEqual(len(self.ids), len(docs))
        self.assertEqual(len(self.ids), docs.update_seq)

    def test_check_document_update_sequence_malformed(self):
        docs = couch[self.test_db_name].view('_all_docs', update_seq = 'Tru')
        # have to query the docs before a request is actually sent
        with self.assertRaises(couchdb.ServerError) as se:
            self.assertEqual(len(self.ids), len(docs))

        self.assertEqual(se.exception[0][0], 400)
        self.assertEqual(se.exception[0][1], (u'query_parse_error', u'Invalid value for boolean: "Tru"'))

    def test_check_document_update_sequence_with_keys(self):
        docs = couch[self.test_db_name].view('_all_docs', keys = self.ids, update_seq = True)
        self.assertEqual(len(self.ids), len(docs))
        self.assertEqual(len(self.ids), docs.update_seq)

    def test_check_document_update_sequence_with_keys_malformed(self):
        docs = couch[self.test_db_name].view('_all_docs', keys = self.ids, update_seq = 'Tru')
        # have to query the docs before a request is actually sent
        with self.assertRaises(couchdb.ServerError) as se:
            self.assertEqual(len(self.ids), len(docs))

        self.assertEqual(se.exception[0][0], 400)
        self.assertEqual(se.exception[0][1], (u'query_parse_error', u'Invalid value for boolean: "Tru"'))

    def test_check_document_count_order_with_include_docs(self):
        docs = couch[self.test_db_name].view('_all_docs', include_docs = True)
        self.assertEqual(len(self.ids), len(docs))
        for i in xrange(len(self.ids)):
            doc_id = self.ids[i]
            self.assertEqual(doc_id, docs.rows[i].id)
            self.assertEqual(doc_id, docs.rows[i].doc.id)

    def test_check_limit(self):
        docs = couch[self.test_db_name].view('_all_docs', limit = 1)
        self.assertEqual(1, len(docs))
        self.assertEqual(self.ids[0], docs.rows[0].id)

    def test_check_negative_limit(self):
        docs = couch[self.test_db_name].view('_all_docs', limit = -1)
        # have to query the docs before a request is actually sent
        with self.assertRaises(couchdb.ServerError) as se:
            self.assertEqual(len(self.ids), len(docs))

        self.assertEqual(se.exception[0][0], 400)
        self.assertEqual(se.exception[0][1], (u'query_parse_error', u'Invalid value for positive integer: "-1"'))

    def test_check_negative_skip(self):
        docs = couch[self.test_db_name].view('_all_docs', skip = -1)
        # have to query the docs before a request is actually sent
        with self.assertRaises(couchdb.ServerError) as se:
            self.assertEqual(len(self.ids), len(docs))

        self.assertEqual(se.exception[0][0], 400)
        self.assertEqual(se.exception[0][1], (u'query_parse_error', u'Invalid value for positive integer: "-1"'))

    def test_check_limit1_skip1(self):
        docs = couch[self.test_db_name].view('_all_docs', limit = 1, skip = 1)
        self.assertEqual(1, len(docs))
        self.assertEqual(self.ids[1], docs.rows[0].id)

    def test_check_limit1_skip1(self):
        docs = couch[self.test_db_name].view('_all_docs', limit = 2, skip = 1)
        self.assertEqual(2, len(docs))
        self.assertEqual(self.ids[1], docs.rows[0].id)
        self.assertEqual(self.ids[2], docs.rows[1].id)

    def test_check_limit5_skip2(self):
        docs = couch[self.test_db_name].view('_all_docs', limit = 5, skip = 2)
        self.assertEqual(5, len(docs))
        self.assertEqual(self.ids[2], docs.rows[0].id)
        self.assertEqual(self.ids[3], docs.rows[1].id)
        self.assertEqual(self.ids[4], docs.rows[2].id)
        self.assertEqual(self.ids[5], docs.rows[3].id)
        self.assertEqual(self.ids[6], docs.rows[4].id)

    def test_check_descending(self):
        docs = couch[self.test_db_name].view('_all_docs', descending = True)
        self.assertEqual(len(self.ids), len(docs))

        len_ids = len(self.ids)
        for i in xrange(len_ids):
            self.assertEqual(self.ids[len_ids - (i + 1)], docs.rows[i].id)

    def test_check_descending_limit1(self):
        docs = couch[self.test_db_name].view('_all_docs', descending = True, limit = 1)
        self.assertEqual(1, len(docs))

        len_ids = len(self.ids)
        self.assertEqual(self.ids[len_ids - 1], docs.rows[0].id)

    def test_check_descending_limit2(self):
        docs = couch[self.test_db_name].view('_all_docs', descending = True, limit = 2)
        self.assertEqual(2, len(docs))

        len_ids = len(self.ids)
        self.assertEqual(self.ids[len_ids - 1], docs.rows[0].id)
        self.assertEqual(self.ids[len_ids - 2], docs.rows[1].id)

    def test_check_descending_limit1_skip1(self):
        docs = couch[self.test_db_name].view('_all_docs', descending = True, limit = 1, skip = 1)
        self.assertEqual(1, len(docs))

        len_ids = len(self.ids)
        self.assertEqual(self.ids[len_ids - 2], docs.rows[0].id)

    def test_check_descending_limit2_skip4(self):
        docs = couch[self.test_db_name].view('_all_docs', descending = True, limit = 2, skip = 4)
        self.assertEqual(2, len(docs))

        len_ids = len(self.ids)
        self.assertEqual(self.ids[len_ids - 5], docs.rows[0].id)
        self.assertEqual(self.ids[len_ids - 6], docs.rows[1].id)

    def test_check_key(self):
        docs = couch[self.test_db_name].view('_all_docs', key = '3')
        self.assertEqual(1, len(docs))

        idx = self.ids.index('3')
        self.assertEqual(self.ids[idx], docs.rows[0].id)

    def test_check_key_limit1_skip0(self):
        docs = couch[self.test_db_name].view('_all_docs', key = '3', limit = 1, skip = 0)
        self.assertEqual(1, len(docs))

        idx = self.ids.index('3')
        self.assertEqual(self.ids[idx], docs.rows[0].id)

    def test_check_key_limit1_skip1(self):
        docs = couch[self.test_db_name].view('_all_docs', key = '3', limit = 1, skip = 1)
        self.assertEqual(0, len(docs))

    def test_check_missed_key(self):
        docs = couch[self.test_db_name].view('_all_docs', key = 'x')
        self.assertEqual(0, len(docs))

    def test_check_start_key(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '3')
        
        idx = self.ids.index('3')
        self.assertEqual(len(self.ids) - idx, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(self.ids[idx], docs.rows[i].id)
            idx += 1

    def test_check_start_key_descending(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '3', descending = True)

        rev_ids = self.ids[:]
        rev_ids.reverse()
        idx = rev_ids.index('3')
        self.assertEqual(len(rev_ids) - idx, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(rev_ids[idx], docs.rows[i].id)
            idx += 1

    def test_check_start_key_descending_limit1(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '3', descending = True, limit = 1)
        self.assertEqual(1, len(docs))
        self.assertEqual('3', docs.rows[0].id)

    def test_check_start_key_descending_limit1_skip1(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '3', descending = True, limit = 1, skip = 1)
        self.assertEqual(1, len(docs))
        self.assertEqual('2', docs.rows[0].id)

    def test_check_end_key(self):
        docs = couch[self.test_db_name].view('_all_docs', endkey = '3')
        
        idx = self.ids.index('3')
        self.assertEqual(idx + 1, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(self.ids[i], docs.rows[i].id)

    def test_check_end_key_no_inclusive_end(self):
        docs = couch[self.test_db_name].view('_all_docs', endkey = '3', inclusive_end = False)
        
        idx = self.ids.index('3')
        self.assertEqual(idx, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(self.ids[i], docs.rows[i].id)

    def test_check_end_key_limit1(self):
        docs = couch[self.test_db_name].view('_all_docs', endkey = '3', limit = 1)
        
        self.assertEqual(1, len(docs))
        self.assertEqual(self.ids[0], docs.rows[0].id)

    def test_check_end_key_limit3(self):
        docs = couch[self.test_db_name].view('_all_docs', endkey = '3', limit = 3)
        
        self.assertEqual(3, len(docs))
        self.assertEqual(self.ids[0], docs.rows[0].id)
        self.assertEqual(self.ids[1], docs.rows[1].id)
        self.assertEqual(self.ids[2], docs.rows[2].id)

    def test_check_end_key_descending(self):
        docs = couch[self.test_db_name].view('_all_docs', endkey = '3', descending = True)
        
        rev_ids = self.ids[:]
        rev_ids.reverse()
        idx = rev_ids.index('3')
        self.assertEqual(idx + 1, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(rev_ids[i], docs.rows[i].id)

    def test_check_end_key_descending_no_inclusive_end(self):
        docs = couch[self.test_db_name].view('_all_docs', endkey = '3', descending = True, inclusive_end = False)
        
        rev_ids = self.ids[:]
        rev_ids.reverse()
        idx = rev_ids.index('3')
        self.assertEqual(idx, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(rev_ids[i], docs.rows[i].id)

    def test_check_end_key_descending_limit1(self):
        docs = couch[self.test_db_name].view('_all_docs', endkey = '3', descending = True, limit = 1)
        self.assertEqual(1, len(docs))
        self.assertEqual(self.ids[len(self.ids) - 1], docs.rows[0].id)

    def test_check_end_key_descending_limit1_skip1(self):
        docs = couch[self.test_db_name].view('_all_docs', endkey = '3', descending = True, limit = 1, skip = 1)
        self.assertEqual(1, len(docs))
        self.assertEqual(self.ids[len(self.ids) - 2], docs.rows[0].id)

    def test_check_end_key_descending_limit20_skip2(self):
        docs = couch[self.test_db_name].view('_all_docs', endkey = '3', descending = True, limit = 20, skip = 2)
        
        rev_ids = self.ids[:]
        rev_ids.reverse()
        idx = rev_ids.index('3')
        self.assertEqual(idx - 1, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(rev_ids[i + 2], docs.rows[i].id)

    def test_check_start_key_end_key(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '0', endkey = '3')
        
        idx = self.ids.index('3')
        self.assertEqual(idx + 1, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(self.ids[i], docs.rows[i].id)

    def test_check_start_key_end_key_not_inclusive_end(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '0', endkey = '3', inclusive_end = False)
        
        idx = self.ids.index('3')
        self.assertEqual(idx, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(self.ids[i], docs.rows[i].id)

    def test_check_start_key_end_key_limit1(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '0',endkey = '3', limit = 1)
        self.assertEqual(1, len(docs))
        self.assertEqual(self.ids[0], docs.rows[0].id)

    def test_check_start_key_end_key_limit1_skip1(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '0',endkey = '3', limit = 1, skip = 1)
        self.assertEqual(1, len(docs))
        self.assertEqual(self.ids[1], docs.rows[0].id)

    def test_check_start_key_end_key(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '1', endkey = '3')
        
        idx = self.ids.index('1')
        idx_2 = self.ids.index('3')
        self.assertEqual((idx_2 - idx) + 1, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(self.ids[idx + i], docs.rows[i].id)

    def test_check_start_key_end_key_descending(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '0',endkey = '3', descending = True)
        self.assertEqual(0, len(docs))

    def test_check_reverse_start_key_end_key_descending(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '3',endkey = '0', descending = True)

        rev_ids = self.ids[:]
        rev_ids.reverse()
        idx = rev_ids.index('3')
        idx_2 = rev_ids.index('0')
        self.assertEqual((idx_2 - idx) + 1, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(rev_ids[idx + i], docs.rows[i].id)

    def test_check_reverse_start_key_end_key_descending(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '3', endkey = '1', descending = True)

        rev_ids = self.ids[:]
        rev_ids.reverse()
        idx = rev_ids.index('3')
        idx_2 = rev_ids.index('1')
        self.assertEqual((idx_2 - idx) + 1, len(docs))

        for i in xrange(len(docs)):
            self.assertEqual(rev_ids[idx + i], docs.rows[i].id)

    def test_check_reverse_start_key_end_key_descending_limit1(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '3', endkey = '1', descending = True, limit = 1)
        self.assertEqual(1, len(docs))
        rev_ids = self.ids[:]
        rev_ids.reverse()
        idx = rev_ids.index('3')
        self.assertEqual(rev_ids[idx], docs.rows[0].id)

    def test_check_reverse_start_key_end_key_descending_limit1_skip1(self):
        docs = couch[self.test_db_name].view('_all_docs', startkey = '3', endkey = '1', descending = True, limit = 1, skip = 1)
        self.assertEqual(1, len(docs))
        rev_ids = self.ids[:]
        rev_ids.reverse()
        idx = rev_ids.index('3')
        self.assertEqual(rev_ids[idx + 1], docs.rows[0].id)

    def test_check_with_keys(self):
        keys = self.ids[:4]
        docs = couch[self.test_db_name].view('_all_docs', keys = keys)
        self.assertEqual(len(keys), len(docs))

        for i in xrange(len(keys)):
            self.assertEqual(keys[i], docs.rows[i].id)

    def test_check_with_keys_include_docs(self):
        keys = self.ids[:4]
        docs = couch[self.test_db_name].view('_all_docs', keys = keys, include_docs = True)
        self.assertEqual(len(keys), len(docs))

        for i in xrange(len(keys)):
            self.assertEqual(keys[i], docs.rows[i].id)
            self.assertEqual(keys[i], docs.rows[i].doc.id)

    def test_check_with_keys_include_docs_malformed(self):
        keys = self.ids[:4]
        docs = couch[self.test_db_name].view('_all_docs', keys = keys, include_docs = 'Tru')
        # have to query the docs before a request is actually sent
        with self.assertRaises(couchdb.ServerError) as se:
            self.assertEqual(len(self.ids), len(docs))

        self.assertEqual(se.exception[0][0], 400)
        self.assertEqual(se.exception[0][1], (u'query_parse_error', u'Invalid value for boolean: "Tru"'))

    def test_check_with_keys_descending(self):
        keys = self.ids[:4]
        docs = couch[self.test_db_name].view('_all_docs', keys = keys, descending = True)
        self.assertEqual(len(keys), len(docs))

        for i in xrange(len(keys)):
            self.assertEqual(keys[i], docs.rows[len(keys) - 1 - i].id)

    def test_check_with_keys_limit_10(self):
        keys = self.ids[:4]
        docs = couch[self.test_db_name].view('_all_docs', keys = keys, limit = 10)
        self.assertEqual(len(keys), len(docs))

        for i in xrange(len(keys)):
            self.assertEqual(keys[i], docs.rows[i].id)

    def test_check_with_keys_limit_1(self):
        keys = self.ids[:4]
        docs = couch[self.test_db_name].view('_all_docs', keys = keys, limit = 1)
        self.assertEqual(1, len(docs))
        self.assertEqual(keys[0], docs.rows[0].id)

    def test_check_with_keys_limit_1_skip_1(self):
        keys = self.ids[:4]
        docs = couch[self.test_db_name].view('_all_docs', keys = keys, limit = 1, skip = 1)
        self.assertEqual(1, len(docs))
        self.assertEqual(keys[1], docs.rows[0].id)

    def test_check_with_keys_limit_1_skip_100(self):
        keys = self.ids[1:4]
        docs = couch[self.test_db_name].view('_all_docs', keys = keys, limit = 1, skip = 100)
        self.assertEqual(0, len(docs))

    def test_check_with_all_keys(self):
        keys = self.ids
        docs = couch[self.test_db_name].view('_all_docs', keys = keys)
        self.assertEqual(len(keys), len(docs))

        for i in xrange(len(keys)):
            self.assertEqual(keys[i], docs.rows[i].id)

    def test_check_with_all_keys_negative_skip(self):
        keys = self.ids
        docs = couch[self.test_db_name].view('_all_docs', keys = keys, skip = -1)
        # have to query the docs before a request is actually sent
        with self.assertRaises(couchdb.ServerError) as se:
            self.assertEqual(len(self.ids), len(docs))

        self.assertEqual(se.exception[0][0], 400)
        self.assertEqual(se.exception[0][1], (u'query_parse_error', u'Invalid value for positive integer: "-1"'))

    def test_check_with_all_keys_negative_limit(self):
        keys = self.ids
        docs = couch[self.test_db_name].view('_all_docs', keys = keys, limit = -1)
        # have to query the docs before a request is actually sent
        with self.assertRaises(couchdb.ServerError) as se:
            self.assertEqual(len(self.ids), len(docs))

        self.assertEqual(se.exception[0][0], 400)
        self.assertEqual(se.exception[0][1], (u'query_parse_error', u'Invalid value for positive integer: "-1"'))

    def test_check_with_all_keys_descending(self):
        keys = self.ids
        docs = couch[self.test_db_name].view('_all_docs', keys = keys, descending = True)
        self.assertEqual(len(keys), len(docs))

        for i in xrange(len(keys)):
            self.assertEqual(keys[i], docs.rows[len(keys) - 1 - i].id)

    def test_check_with_keys_include_docs_malformed(self):
        keys = self.ids
        docs = couch[self.test_db_name].view('_all_docs', keys = keys, include_docs = 'Tru')
        # have to query the docs before a request is actually sent
        with self.assertRaises(couchdb.ServerError) as se:
            self.assertEqual(len(self.ids), len(docs))

        self.assertEqual(se.exception[0][0], 400)
        self.assertEqual(se.exception[0][1], (u'query_parse_error', u'Invalid value for boolean: "Tru"'))

    def test_check_with_all_keys_modified_first_missing_doc(self):
        keys = self.ids[:1]
        keys[0] = 'abc'
        docs = couch[self.test_db_name].view('_all_docs', keys = keys)
        self.assertEqual(len(keys), len(docs))

        for i in xrange(len(keys)):
            self.assertEqual(keys[i], docs.rows[i].key)

        self.assertEqual(u'not_found', docs.rows[i].error)

    def test_check_with_all_keys_added_last_missing_doc(self):
        keys = self.ids[:]
        keys.append('abc')
        docs = couch[self.test_db_name].view('_all_docs', keys = keys)
        self.assertEqual(len(keys), len(docs))

        for i in xrange(len(keys)):
            self.assertEqual(keys[i], docs.rows[i].key)

        self.assertEqual(u'not_found', docs.rows[len(docs) - 1].error)

    def test_check_badly_typed_keys(self):
        keys = [1, None, True, False, 'wawawewa', {}, []];
        docs = couch[self.test_db_name].view('_all_docs', keys = keys)
        self.assertEqual(len(keys), len(docs))

        for i in xrange(len(keys)):
            self.assertEqual(keys[i], docs.rows[i].key)
            self.assertEqual(u'not_found', docs.rows[i].error)



if __name__ == "__main__":
    unittest.main() # run all tests
