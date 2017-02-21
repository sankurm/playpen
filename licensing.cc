// Example program
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>
#include <boost/variant.hpp>

using namespace std;
using boost::variant;

enum class license_status { pending, positive, error };
ostream& operator<<(ostream& out, license_status status) {
    out << (status == license_status::pending? "Pending": (status == license_status::positive? "Positive": "Error"));
    return out;
}

class boolean_request
{
    string name;
    public:
        boolean_request(string _name) : name(move(_name)) {}
        const string& get_name() const { return name; }
};

class counted_request
{
    string name;
    int requested_count;
    public:
        counted_request(string _name, int count_requested) : name(move(_name)), requested_count(count_requested) {}
        const string& get_name() const { return name; }
        int get_requested_count() const { return requested_count; }
};

class threshold_request : public boolean_request
{
    //Created just to have two different types
    public:
        threshold_request(string name) : boolean_request(name) {};
};

ostream& operator<<(ostream& out, const boolean_request& req) {
    out << '{' << req.get_name() << '}';
    return out;
}
ostream& operator<<(ostream& out, const threshold_request& req) {
    out << '{' << req.get_name() << '}';
    return out;
}
ostream& operator<<(ostream& out, const counted_request& req) {
    out << '{' << req.get_name() << ", requested_count=" << req.get_requested_count() << '}';
    return out;
}

using license_request = variant<boolean_request, threshold_request, counted_request>;

class basic_response
{
    string name;
    int expiry;
    public:
        basic_response(string _name, int _expiry) : name(move(_name)), expiry(_expiry) {}
        const string& get_name() const { return name; }
        int get_expiry() const { return expiry; }
};

using boolean_response = basic_response;

class threshold_response : public basic_response
{
    int threshold_value;
    public:
        threshold_response(string name, int expiry, int _threshold_value) : basic_response(name, expiry), threshold_value(_threshold_value) {}
        int get_threshold_value() const { return threshold_value; }
};

class counted_response : public basic_response
{
    int granted_count;
    public:
        counted_response(string name, int expiry, int _granted_count) : basic_response(name, expiry), granted_count(_granted_count) {}
        int get_granted_count() const { return granted_count; }
};

class error_response
{
    string name;
    license_status status;
    string error_description;
    int available_count;
    public:
        error_response(string _name, license_status _status, string _error_description) : name(move(_name)), status(_status), error_description(move(_error_description)) {}
        const string& get_name() const { return name; }
        license_status get_status() const { return status; }
        const string& get_error_description() const { return error_description; }
        int get_available_count() const { return available_count; }
};

using license_response = variant<boolean_response, threshold_response, counted_response, error_response>;

ostream& operator<<(ostream& out, const boolean_response& resp) {
    out << '{' << resp.get_name() << ", expiry=" << resp.get_expiry() << '}';
    return out;
}
ostream& operator<<(ostream& out, const threshold_response& resp) {
    out << '{' << resp.get_name() << ", expiry=" << resp.get_expiry() << ", threshold_value=" << resp.get_threshold_value() << '}';
    return out;
}
ostream& operator<<(ostream& out, const counted_response& resp) {
    out << '{' << resp.get_name() << ", expiry=" << resp.get_expiry() << ", granted_count=" << resp.get_granted_count() << '}';
    return out;
}
ostream& operator<<(ostream& out, const error_response& resp) {
    out << '{' << resp.get_name() << ", status=" << resp.get_status() << ", error_description=" << resp.get_error_description() << '}';
    return out;
}

using ip = string;

class license_server
{
    ip ip_address;
    int port;
    bool is_secured;
    public:
        license_server(ip address, int _port, bool secured) : ip_address(address), port(_port), is_secured(secured) {}
        //license_response send(license_request req) { return counted_response("STR"s, 24, 1000); }
        license_response send(boolean_request req) { return boolean_response(req.get_name(), 24); }
        license_response send(threshold_request req) { return threshold_response(req.get_name(), 48, 2000); }
        license_response send(counted_request req) { 
            if (req.get_name() != "COUNTED2")
                return counted_response(req.get_name(), 96, req.get_requested_count());
            else
                return error_response(req.get_name(), license_status::error, "Server is in bad mood!"s/*req.get_requested_count() - 5*/);
        }
};

class request_dispatcher : public boost::static_visitor<license_response>
{
    license_server &server;
    public:
        request_dispatcher(license_server& _server) : server(_server) {}
        template<typename request_type>
        license_response operator()(request_type &req) const { cout << "request: " << req << endl; return server.send(req); } 
};

class license_client
{
    ip local;
    int port;
    int retry_interval;
    vector<license_server> connected_servers;
    public:
        license_client(ip _local, int _port, int _retry_interval) : local(_local), port(_port), retry_interval(_retry_interval) {}
        bool connect(license_server server) { connected_servers.push_back(server); return true; }
        license_response request_license(license_request request) { return boost::apply_visitor(request_dispatcher(connected_servers.front()), request); }
        void handle_response(boolean_response resp) { cout << "handle_response: " << resp << endl << endl; }
        void handle_response(threshold_response resp) { cout << "handle_response: " << resp << endl << endl; }
        void handle_response(counted_response resp) { cout << "handle_response: " << resp << endl << endl; }
        void handle_response(error_response resp) { cout << "handle_response: " << resp << endl << endl; }
};

class response_handler : public boost::static_visitor<>
{
    license_client &client;
    public:
        response_handler(license_client& c) : client(c) {}
        template<typename response_type>
        void operator()(response_type& resp) const { cout << "response: " << resp << endl; return client.handle_response(resp); }
};

int main()
{
    license_server server("10.10.10.1", 7000, true);
    license_client client("10.0.0.1", 10001, 300);
    client.connect(server);
    
    license_request counted_req = counted_request("COUNTED1", 20);
    auto cresp = client.request_license(counted_req);
    boost::apply_visitor(response_handler(client), cresp);
    //cout << "counted response: " << cresp << endl;
    
    license_request boolean_req = boolean_request("BOOL1");
    auto bresp = client.request_license(boolean_req);
    boost::apply_visitor(response_handler(client), bresp);
    //cout << "boolean response: " << bresp << endl;
    
    license_request threshold_req = threshold_request("THRESHOLD1");
    auto tresp = client.request_license(threshold_req);
    boost::apply_visitor(response_handler(client), tresp);
    //cout << "threshold response: " << tresp << endl;
    
    license_request counted_req2 = counted_request("COUNTED2", 10000);
    auto eresp = client.request_license(counted_req2);
    boost::apply_visitor(response_handler(client), eresp);
    //cout << "error response: " << eresp << endl;
    
    //auto resp = server.send(req);
    //cout << "response: " << resp << endl;
    return 0;
}
