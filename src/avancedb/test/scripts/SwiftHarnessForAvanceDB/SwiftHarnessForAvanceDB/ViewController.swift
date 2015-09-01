//
//  ViewController.swift
//  SwiftHarnessForAvanceDB
//
//  Created by Simon Stearn on 01/09/2015.
//  Copyright © 2015 Simon Stearn. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var url: UITextField!
    @IBOutlet weak var user: UITextField!
    @IBOutlet weak var password: UITextField!
    
    @IBAction func RunTest(sender: AnyObject) {
        
        
        // Respect defaults and unwrap etc
        var target_url = String(url.text!)
        let target_user = String(user.text!)
        let target_password = String(password.text!)
        
        if target_url == "" {
            target_url = "http://localhost:5984"
        }
       
        // create new client
        let couchdb = CouchDB(url: target_url, name: target_user, password: target_password)
        print("Connecting to \"\(target_url)\" as \"\(target_user)\" with pwd \"\(target_password)\".")
        
        // use database
        let database = couchdb.use("test_db")
        
        // Connected - note in log
        print("Connection Successful!")
        
        // get document by id
//        database.get("abc123") { response in
//            switch response {
//            case .Error(let error):
//                print(error)
//            case .Success(let data):
//                print(data)
//            }
//        }
        
        // Segue into next view and present tests
        
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    


}

