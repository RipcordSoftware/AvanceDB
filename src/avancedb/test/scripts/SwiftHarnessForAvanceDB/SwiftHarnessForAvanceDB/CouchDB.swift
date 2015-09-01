
import Foundation



private let DOMAIN = "CouchDB"



public class CouchDB {
    
    private var url: String
    private var headers: [String: String]?
    
    public init(url: String, name: String?, password: String?) {
        self.url = url.hasSuffix("/") ? url : "\(url)/"
        if let name = name, password = password {
            let auth = "\(name):\(password)"
            let data = auth.dataUsingEncoding(NSUTF8StringEncoding)
            let base = data!.base64EncodedStringWithOptions([])
            self.headers = [
                "Authorization": "Basic \(base)"
            ]
        }
    }
    
    /**
     * Info
     *
     * http://docs.couchdb.org/en/1.6.1/api/server/common.html#get--
     */
    public struct HTTPInfoResponse {
        public var couchdb: String!
        public var uuid: String!
        public var version: String!
        
        public init(data: AnyObject) {
            if let d = data as? [String: AnyObject] {
                if let
                    couchdb = d["couchdb"] as? String,
                    uuid = d["uuid"] as? String,
                    version = d["version"] as? String {
                        self.couchdb = couchdb
                        self.uuid = uuid
                        self.version = version
                }
            }
        }
    }
    
    public enum InfoResponse {
        case Success(HTTPInfoResponse)
        case Error(NSError)
    }
    
    public func info(done: (InfoResponse) -> Void) {
        
        HTTP.get(self.url, headers: self.headers) { response in
            switch response {
            case .Error(let error):
                done(.Error(error))
            case .Success(let json, let res):
                if res.statusCode != 200 {
                    done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [
                        NSLocalizedDescriptionKey: NSHTTPURLResponse.localizedStringForStatusCode(res.statusCode)
                        ])))
                    return
                }
                done(.Success(HTTPInfoResponse(data: json)))
            }
        }
    }
    
    
    
    /**
     * Login
     *
     * http://docs.couchdb.org/en/latest/api/server/authn.html
     */
    
    /**
    * POST _session response
    *
    * http://docs.couchdb.org/en/latest/api/server/authn.html#post--_session
    */
    public struct HTTPPostSessionResponse {
        public var ok: Bool!
        public var name: String!
        public var roles: [String]!
        
        public init(data: AnyObject) {
            if let dict = data as? [String: AnyObject] {
                if let
                    ok = dict["ok"] as? Bool,
                    name = dict["name"] as? String,
                    roles = dict["roles"] as? [String] {
                        self.ok = ok
                        self.name = name
                        self.roles = roles
                }
            }
        }
    }
    
    public enum LoginResponse {
        case Success(HTTPPostSessionResponse)
        case Error(NSError)
    }
    
    public func login(name: String, password: String, done: (LoginResponse) -> Void) {
        let data = [
            "name": name,
            "password": password
        ]
        HTTP.post("\(self.url)_session", headers: self.headers, data: data) { result in
            switch result {
            case .Error(let error):
                done(.Error(error))
            case .Success(let json, let response):
                if response.statusCode != 200 {
                    done(.Error(NSError(domain: DOMAIN, code: response.statusCode, userInfo: [
                        NSLocalizedDescriptionKey: NSHTTPURLResponse.localizedStringForStatusCode(response.statusCode)
                    ])))
                    return
                }
                done(.Success(HTTPPostSessionResponse(data: json)))
            }
        }
    }
    
    
    
    /**
     * Get session
     * 
     * http://docs.couchdb.org/en/latest/api/server/authn.html#get--_session
     */
    
    // http://docs.couchdb.org/en/latest/json-structure.html#user-context-object
    public struct UserContext {
        public var name: String!
        public var roles: [String]!
        
        public init(data: AnyObject) {
            if let d = data as? [String: AnyObject] {
                if let
                    name = d["name"] as? String,
                    roles = d["roles"] as? [String] {
                        self.name = name
                        self.roles = roles
                }
            }
        }
    }
    
    public struct HTTPGetSessionResponseInfo {
        public var authenticated: String!
        public var authentication_db: String!
        public var authentication_handlers: [String]!
        
        public init(data: AnyObject) {
            if let d = data as? [String: AnyObject] {
                if let
                    authenticated = d["authenticated"] as? String,
                    authentication_db = d["authentication_db"] as? String,
                    authentication_handlers = d["authentication_handlers"] as? [String] {
                        self.authenticated = authenticated
                        self.authentication_db = authentication_db
                        self.authentication_handlers = authentication_handlers
                }
                
            }
        }
    }
    
    public struct HTTPGetSessionResponse {
        public var info: HTTPGetSessionResponseInfo!
        public var ok: Bool!
        public var userCtx: UserContext!
        
        public init(data: AnyObject) {
            if let d = data as? [String: AnyObject] {
                if let
                    info = d["info"] as? [String: AnyObject],
                    ok = d["ok"] as? Bool,
                    userCtx = d["userCtx"] as? [String: AnyObject] {
                        self.info = HTTPGetSessionResponseInfo(data: info)
                        self.ok = ok
                        self.userCtx = UserContext(data: userCtx)
                }
            }
        }
    }
    
    public enum GetSessionResponse {
        case Success(HTTPGetSessionResponse)
        case Error(NSError)
    }
    
    public func getSession(done: (GetSessionResponse) -> Void) {
        HTTP.get("\(self.url)_session") { response in
            switch response {
            case .Error(let error):
                done(.Error(error))
            case .Success(let json, let res):
                if res.statusCode != 200 {
                    done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [
                        NSLocalizedDescriptionKey: NSHTTPURLResponse.localizedStringForStatusCode(res.statusCode)
                    ])))
                    return
                }
                done(.Success(HTTPGetSessionResponse(data: json)))
            }
        }
    }
    
    
    
    /**
     * Logout
     *
     * http://docs.couchdb.org/en/latest/api/server/authn.html#delete--_session
     */
    public struct HTTPDeleteSessionResponse {
        public var ok: Bool!
        
        public init(data: AnyObject) {
            if let d = data as? [String: AnyObject] {
                if let ok = d["ok"] as? Bool {
                    self.ok = ok
                }
            }
        }
    }
    
    public enum LogoutResponse {
        case Success(HTTPDeleteSessionResponse)
        case Error(NSError)
    }
    
    public func logout(done: (LogoutResponse) -> Void) {
        HTTP.delete("\(self.url)_session") { response in
            switch response {
            case .Error(let error):
                done(.Error(error))
            case .Success(let json, let res):
                if res.statusCode != 200 {
                    done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [
                        NSLocalizedDescriptionKey: NSHTTPURLResponse.localizedStringForStatusCode(res.statusCode)
                    ])))
                    return
                }
                done(.Success(HTTPDeleteSessionResponse(data: json)))
            }
        }
    }
    
    
    
    /**
     * Create database
     *
     * http://docs.couchdb.org/en/latest/api/database/common.html#put--db
     */
    
    public struct HTTPPutCreateSuccess {
        public var ok: Bool!
        
        public init(data: AnyObject) {
            if let dict = data as? [String: AnyObject] {
                if
                    let ok = dict["ok"] as? Bool {
                        self.ok = ok
                }
            }
        }
    }
    
    public enum CreateDatabaseResponse {
        case Success(HTTPPutCreateSuccess)
        case Error(NSError)
    }
    
    public func createDatabase(database: String, done: (CreateDatabaseResponse) -> Void) {
        HTTP.put("\(self.url)\(database)") { response in
            switch response {
            case .Error(let error):
                done(.Error(error))
            case .Success(let json, let res):
                if res.statusCode != 201 {
                    done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [
                        NSLocalizedDescriptionKey: NSHTTPURLResponse.localizedStringForStatusCode(res.statusCode)
                    ])))
                    return
                }
                done(.Success(HTTPPutCreateSuccess(data: json)))
            }
        }
    }
    
    
    
    /**
     * Delete database
     *
     * http://docs.couchdb.org/en/latest/api/database/common.html#delete--db
     */
    
    public struct HTTPDeleteResponse {
        public var ok: Bool!
        
        public init(data: AnyObject) {
            if let dict = data as? [String: AnyObject] {
                if let ok = dict["ok"] as? Bool {
                    self.ok = ok
                }
            }
        }
    }
    
    public enum DeleteDatabaseReponse {
        case Success(HTTPDeleteResponse)
        case Error(NSError)
    }
    
    public func deleteDatabase(database: String, done: (DeleteDatabaseReponse) -> Void) {
        HTTP.delete("\(self.url)\(database)") { response in
            switch response {
            case .Error(let error):
                done(.Error(error))
            case .Success(let json, let res):
                if res.statusCode != 200 {
                    done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [:])))
                    return
                }
                done(.Success(HTTPDeleteResponse(data: json)))
            }
            
        }
    }
    
    
    
    /**
     * Create user
     */
    
    /**
     * User struct
     * 
     * http://docs.couchdb.org/en/latest/intro/security.html#creating-new-user
     */
    public struct User {
        public var name: String!
        public var password: String!
        public var roles: [String]!
        private let type: String = "user"
        
        public init(name: String, password: String, roles: [String]) {
            self.name = name
            self.password = password
            self.roles = roles
        }
        
        public func serialize() -> [String: AnyObject] {
            var dict = [String: AnyObject]()
            dict["name"] = name
            dict["password"] = password
            dict["roles"] = roles
            dict["type"] = type
            return dict
        }
    }
    
    /**
     * Create user in db
     */
    public func createUser(user: User, done: (Database.PostResponse) -> Void) {
        let data = user.serialize()
        HTTP.put("\(self.url)_users/org.couchdb.user:\(user.name)", data: data) { response in
            switch response {
            case .Error(let error):
                done(.Error(error))
            case .Success(let json, let res):
                if res.statusCode != 201 {
                    done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [
                        NSLocalizedDescriptionKey: NSHTTPURLResponse.localizedStringForStatusCode(res.statusCode)
                    ])))
                    return
                }
                done(.Success(Database.HTTPPostDatabaseReponse(data: json)))
            }
        }
    }
    
    
    
    /**
     * Use database
     */
    public func use(name: String) -> Database {
        return Database(url: "\(self.url)\(name)", headers: self.headers)
    }
    
    
    
    /**
     * Document
     */
    public class Document {
        
        public var dictionary = [String: AnyObject]()
        public var _id: String?
        public var _rev: String?
        
        public init(_id: String?, _rev: String?) {
            self._id = _id
            self._rev = _rev
        }
        
        public init(data: AnyObject) {
            if let
                _id = data["_id"] as? String,
                _rev = data["_rev"] as? String {
                    self._id = _id
                    self._rev = _rev
            }
        }
        
        public func serialize() -> [String: AnyObject] {
            self.dictionary["_id"] = self._id
            self.dictionary["_rev"] = self._rev
            return self.dictionary
        }
    }
    
    
    
    /**
     * Design document
     */
    public class DesignDocument: Document {
        
        public let language: String = "javascript"
        public var views: [String: DesignDocumentView]
        
        public init(_id: String?, _rev: String?, views: [String: DesignDocumentView]) {
            self.views = views
            super.init(_id: "_design/\(_id!)", _rev: _rev)
        }
        
        public override func serialize() -> [String : AnyObject] {
            self.dictionary["language"] = language
            var wrapper = [String: AnyObject]()
            for (key, value) in self.views {
                var _views = ["map": value.map]
                if let reduce = value.reduce {
                    _views["reduce"] = reduce
                }
                wrapper[key] = _views
            }
            self.dictionary["views"] = wrapper
            return super.serialize()
        }
        
    }
    
    
    
    /**
     * View
     */
    public class DesignDocumentView {
        public var map: String
        public var reduce: String?
        
        public init(map: String, reduce: String?) {
            self.map = map
            self.reduce = reduce
        }
        
    }
    
    
    
    /**
     * Query params
     *
     * http://docs.couchdb.org/en/latest/api/ddoc/views.html#get--db-_design-ddoc-_view-view
     */
    public class QueryParameters: QueryString {
        
        public var conflicts: Bool?
        public var descending: Bool?
        public var endkey: String?
        public var endkey_docid: String?
        public var group: Bool?
        public var group_level: Int?
        public var include_docs: Bool?
        public var attachments: Bool?
        public var att_encoding_info: Bool?
        public var inclusive_end: Bool?
        public var key: String?
        public var keys: [String]?
        public var limit: Int?
        public var reduce: Bool?
        public var skip: Int?
        public var stale: String?
        public var startkey: String?
        public var startkey_docid: String?
        public var update_seq: Bool?
        
        public override init() {
            super.init()
        }
        
    }
    
    
    
    /**
     * Database
     */
    public class Database {
        
        private var url: String
        private var headers: [String: String]?
        
        public init(url: String, headers: [String: String]?) {
            self.url = url.hasSuffix("/") ? url : "\(url)/"
            self.headers = headers
        }
        
        
        /**
        * Create document
        *
        * http://docs.couchdb.org/en/latest/api/database/common.html#post--db
        */
        public struct HTTPPostDatabaseReponse {
            public var id: String!
            public var ok: Bool!
            public var rev: String!
            
            public init(data: AnyObject) {
                if let dict = data as? [String: AnyObject] {
                    if
                        let id = dict["id"] as? String,
                        let ok = dict["ok"] as? Bool,
                        let rev = dict["rev"] as? String {
                            self.id = id
                            self.ok = ok
                            self.rev = rev
                    }
                }
            }
        }
        
        public enum PostResponse {
            case Success(HTTPPostDatabaseReponse)
            case Error(NSError)
        }
        
        public func post(document: Document, done: (PostResponse) -> Void) {
            HTTP.post(self.url, data: document.serialize()) { response in
                switch response {
                case .Error(let error):
                    done(.Error(error))
                case .Success(let json, let res):
                    if res.statusCode != 201 {
                        done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [:])))
                        return
                    }
                    done(.Success(HTTPPostDatabaseReponse(data: json)))
                }
            }
        }
        
        /**
         * Edit document or create new one with id
         *
         * http://docs.couchdb.org/en/latest/api/document/common.html#put--db-docid
         */
        public func put(document: Document, done: (PostResponse) -> Void) {
            HTTP.put("\(self.url)\(document._id!)", data: document.serialize()) { response in
                switch response {
                case .Error(let error):
                    done(.Error(error))
                case .Success(let json, let res):
                    if res.statusCode != 201 {
                        done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [
                            NSLocalizedDescriptionKey: NSHTTPURLResponse.localizedStringForStatusCode(res.statusCode)
                            ])))
                        return
                    }
                    done(.Success(HTTPPostDatabaseReponse(data: json)))
                }
            }
        }
        
        
        
        /**
         * Delete document
         *
         * http://docs.couchdb.org/en/latest/api/document/common.html#delete--db-docid
         */
        public func delete(document: Document, done: (PostResponse) -> Void) {
            HTTP.delete("\(self.url)\(document._id!)?rev=\(document._rev!)") { response in
                switch response {
                case .Error(let error):
                    done(.Error(error))
                case .Success(let json, let res):
                    if res.statusCode != 200 {
                        done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [
                            NSLocalizedDescriptionKey: NSHTTPURLResponse.localizedStringForStatusCode(res.statusCode)
                            ])))
                        return
                    }
                    done(.Success(HTTPPostDatabaseReponse(data: json)))
                }
            }
        }
        
        
        
        /**
         * Get document
         *
         * http://docs.couchdb.org/en/latest/api/document/common.html#get--db-docid
         */
        public enum GetResponse {
            case Success(AnyObject)
            case Error(NSError)
        }
        
        
        public func get(id: String, done: (GetResponse) -> Void) {
            HTTP.get("\(self.url)\(id)") { response in
                switch response {
                case .Error(let error):
                    done(.Error(error))
                case .Success(let json, let res):
                    if res.statusCode != 200 {
                        done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [
                            NSLocalizedDescriptionKey: NSHTTPURLResponse.localizedStringForStatusCode(res.statusCode)
                            ])))
                        return
                    }
                    done(.Success(json))
                }
            }
        }
        
        
        
        /**
         * Create multiple documents with a single request
         *
         * http://docs.couchdb.org/en/latest/api/database/bulk-api.html
         */
        public struct HTTPBulkResponse {
            public var id: String!
            public var rev: String!
            public var error: String?
            public var reason: String?
            
            public init(data: AnyObject) {
                if let dict = data as? [String: AnyObject] {
                    if let
                        id = dict["id"] as? String,
                        rev = dict["rev"] as? String {
                            self.id = id
                            self.rev = rev
                    }
                    if let error = dict["error"] as? String {
                        self.error = error
                    }
                    if let reason = dict["reason"] as? String {
                        self.reason = reason
                    }
                }
            }
            
        }
        
        public enum BulkResponse {
            case Success([HTTPBulkResponse])
            case Error(NSError)
        }
        
        public func bulk(documents: [Document], done: (BulkResponse) -> Void) {
            let docs = documents.map() { $0.serialize() }
            let data = [
                "docs": docs
            ]
            HTTP.post("\(self.url)_bulk_docs", data: data) { response in
                switch response {
                case .Error(let error):
                    done(.Error(error))
                case .Success(let json, let res):
                    if res.statusCode != 201 {
                        done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [
                            NSLocalizedDescriptionKey: NSHTTPURLResponse.localizedStringForStatusCode(res.statusCode)
                            ])))
                        return
                    }
                    if let data = json as? [AnyObject] {
                        let arr = data.map() { HTTPBulkResponse(data: $0) }
                        done(.Success(arr))
                    }
                }
            }
        }
        
        
        
        /**
         * Use certain view (design document)
         */
        public func view(name: String) -> View {
            return View(url: "\(self.url)_design/\(name)/", headers: self.headers)
        }
        
    }
    
    
    
    /**
     * View
     */
    public class View {
        
        /**
         * http://docs.couchdb.org/en/latest/api/ddoc/views.html#get--db-_design-ddoc-_view-view
         */
        public struct GETResponse {
            public var offset: Int!
            public var rows: [Row]!
            public var total_rows: Int!
            public var update_seq: Int!
            
            public init(data: AnyObject) {
                if let dict = data as? [String: AnyObject] {
                    if
                        let offset = dict["offset"] as? Int,
                        let rows = dict["rows"] as? [AnyObject],
                        let total_rows = dict["total_rows"] as? Int {
                            self.offset = offset
                            self.total_rows = total_rows
                            self.rows = []
                            for row in rows {
                                self.rows.append(Row(data: row))
                            }
                    }
                    if let update_seq = dict["update_seq"] as? Int {
                        self.update_seq = update_seq
                    }
                }
            }
        }
        
        public struct Row {
            public var id: String!
            public var key: String!
            public var value: String!
            
            public init(data: AnyObject) {
                if let dict = data as? [String: AnyObject] {
                    if
                        let id = dict["id"] as? String,
                        let key = dict["key"] as? String {
                            self.id = id
                            self.key = key
                            //                        self.value = value
                            // add doc
                    }
                }
            }
        }
        
        private var url: String
        private var headers: [String: String]?
        
        public init(url: String, headers: [String: String]?) {
            self.url = url.hasSuffix("/") ? url : "\(url)/"
            self.headers = headers
        }
        
        public enum Response {
            case Success(GETResponse)
            case Error(NSError)
        }
        
        public func get(name: String, query: QueryParameters, done: (Response) -> Void) {
            let params = query.encode().stringByAddingPercentEncodingWithAllowedCharacters(.URLQueryAllowedCharacterSet())!
            HTTP.get("\(self.url)_view/\(name)?\(params)", headers: self.headers) { response in
                switch response {
                case .Error(let error):
                    done(.Error(error))
                case .Success(let json, let res):
                    if res.statusCode != 200 {
                        done(.Error(NSError(domain: DOMAIN, code: res.statusCode, userInfo: [
                            NSLocalizedDescriptionKey: NSHTTPURLResponse.localizedStringForStatusCode(res.statusCode)
                            ])))
                        return
                    }
                    done(.Success(GETResponse(data: json)))
                }
            }
        }
    }
    
    
}














