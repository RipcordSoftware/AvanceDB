require 'couchdb'
require 'faker'

# Using localhost
server = CouchDB::Server.new "localhost", 5994
database = CouchDB::Database.new server, "ruby_contact_test"
database.delete_if_exists!
database.create_if_missing!

doc_count = 1000

doc_count.times { |index|

#	if index % 100 == 0 then
		puts "Created #{index} contacts"
#	end


	doc = CouchDB::Document.new database, 
		"first_name" => Faker::Name.first_name,
		"last_name" => Faker::Name.last_name,
		"company_name" => Faker::Company.name,
		"catch_phrase" => Faker::Company.catch_phrase,
		"BS" => Faker::Company.bs,
		"phone_number" => Faker::PhoneNumber.phone_number,
		"email" => Faker::Internet.email,
		"ip_v4_address" => Faker::Internet.ip_v4_address,
		"ip_v6_address" => Faker::Internet.ip_v6_address

	doc.save

}

puts "Created #{doc_count} contacts"
puts "Done."
