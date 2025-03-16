import requests
import json

class ServerConnection:
    is_connected = False
    server_url = "http://localhost:1025/"

    def __init__(self):
        try:
            response = requests.get(self.server_url)
            if response.status_code == 200:
                self.is_connected = True
        except:
            self.is_connected = False
            print("Server not connected")
    
    def get_user_profile(self, username):
        response = requests.get(self.server_url + "user/" + username)
        return response.json()
    
    def get_user_profile_field(self, username, field):
        response = requests.get(self.server_url + "user/" + username + "/" + field)
        return response.json()
    
    def delete_user_profile(self, username):
        response = requests.delete(self.server_url + "user/" + username)
        return response.json()
    
    def delete_user_profile_field(self, username, field):
        response = requests.delete(self.server_url + "user/" + username + "/" + field)
        return response.json()
    
    def post_user_profile(self, profile):
        response = requests.post(self.server_url + "user/", json=profile)
        return response.json()
    
    def put_user_profile(self,username, to_update):
        response = requests.put(self.server_url + "user/" + username, json=to_update)
        return response.json()
    
    def post_user_profile_facts(self, username, facts):
        response = requests.post(self.server_url + "user/" + username + "/facts", json=facts)
        return response.json()

if __name__ == "__main__":
    # Test
    server = ServerConnection()
    assert server.is_connected, "Could not connect to server"
    profile = {"username": "test", "name": "test", "extraversion": 3}
    post_response = server.post_user_profile(profile)

    print("GET: " + json.dumps(server.get_user_profile(username = "test")))
    print("GET field: " + json.dumps(server.get_user_profile_field(username = "test", field = "extraversion")))
    print("PUT: " + json.dumps(server.put_user_profile(username = "test", to_update = {"agreeableness": 5})))
    print("GET: " + json.dumps(server.get_user_profile(username = "test")))
    print("DELETE field: " + json.dumps(server.delete_user_profile_field(username = "test", field = "agreeableness")))
    print("GET: " + json.dumps(server.get_user_profile(username = "test")))
    print("POST facts: " + json.dumps(server.post_user_profile_facts(username = "test", facts = {"fact": "first fact"})))
    print("POST facts: " + json.dumps(server.post_user_profile_facts(username = "test", facts = {"fact": "second fact\nthird fact"})))
    print("GET: " + json.dumps(server.get_user_profile(username = "test")))
    print("DELETE: " + json.dumps(server.delete_user_profile(username = "test")))
    print("GET: " + json.dumps(server.get_user_profile(username = "test")))