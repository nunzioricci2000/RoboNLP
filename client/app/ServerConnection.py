from typing import List, Optional
import requests, json
from ServerResponse import ServerResponse

class ServerConnection:
    instance = None
    is_connected = False
    server_url = "http://localhost:1025/"

    def __new__(self): # Singleton pattern
        if ServerConnection.instance is None:
            ServerConnection.instance = object.__new__(self)
        return ServerConnection.instance

    def __init__(self):
        if not self.is_connected:
            try:
                response = requests.get(self.server_url)
                if response.status_code == 200:
                    self.is_connected = True
            except:
                self.is_connected = False
                print("Server not connected")
    
    def get_user_profile(self, username):
        response = requests.get(self.server_url + "user/" + username)
        if response.status_code == 200:
            ret = ServerResponse.from_profile(response.json())
        elif response.status_code == 404:
            ret = ServerResponse.error("Not found")
        else:
            ret = ServerResponse.error("Error")
        return ret
    
    def get_user_profile_field(self, username, field):
        response = requests.get(self.server_url + "user/" + username + "/" + field)
        if response.status_code == 200:
            ret = ServerResponse.from_profile(response.json())
        elif response.status_code == 404:
            ret = ServerResponse.error("Not found")
        else:
            ret = ServerResponse.error("Error")
        return ret
    
    def delete_user_profile(self, username):
        response = requests.delete(self.server_url + "user/" + username)
        if response.status_code == 200:
            ret = ServerResponse.success("User by the username " + username + "deleted")
        else:
            ret = ServerResponse.error("Error. DELETE operation failed for user " + username)
        return ret
    
    def delete_user_profile_field(self, username, field):
        response = requests.delete(self.server_url + "user/" + username + "/" + field)
        if response.status_code == 200:
            ret = ServerResponse.success("Filed " + field + " for the user " + username + " deleted")
        else:
            ret = ServerResponse.error("Error. DELETE " + field + " operation failed for user " + username)
        return ret
    
    def post_user_profile(self, profile):
        response = requests.post(self.server_url + "user/", json=profile)
        if response.status_code == 200:
            ret = ServerResponse.success("User posted succesfully")
        else:
            ret = ServerResponse.error("Error. POST operation failed.")
        return ret
    
    def put_user_profile(self,username, to_update):
        response = requests.put(self.server_url + "user/" + username, json=to_update)
        if response.status_code == 200:
            ret = ServerResponse.success("User updated succesfully")
        else:
            ret = ServerResponse.error("Error. PUT operation failed")
        return ret
    
    def post_user_profile_facts(self, username, facts):
        response = requests.post(self.server_url + "user/" + username + "/facts", json=facts)
        if response.status_code == 200:
            ret = ServerResponse.success("User fact posted successfully")
        else:
            ret = ServerResponse.error("Error. POST operation failed")
        return ret

if __name__ == "__main__":
    # Test
    server = ServerConnection()
    #assert server.is_connected, "Could not connect to server"
    profile = {"username": "test", "name": "test", "extraversion": 3}
    post_response = server.post_user_profile(profile)

    print("GET: " + server.get_user_profile(username = "test").to_string())
    print("GET field: " + server.get_user_profile_field(username = "test", field = "extraversion").to_string())
    print("PUT: " + server.put_user_profile(username = "test", to_update = {"agreeableness": 5}).to_string())
    print("GET: " + server.get_user_profile(username = "test").user_profile.to_string())
    print("DELETE field: " + server.delete_user_profile_field(username = "test", field = "agreeableness").to_string())
    print("GET: " + server.get_user_profile(username = "test").to_string())
    print("POST facts: " + server.post_user_profile_facts(username = "test", facts = {"fact": "first fact"}).to_string())
    print("POST facts: " + server.post_user_profile_facts(username = "test", facts = {"fact": "second fact\nthird fact"}).to_string())
    print("GET: " + server.get_user_profile(username = "test").to_string())
    print("DELETE: " + server.delete_user_profile(username = "test").to_string())
    print("GET: " + server.get_user_profile(username = "test").to_string())