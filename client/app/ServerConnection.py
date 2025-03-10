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
    
    def delete_user_profile(self, username):
        response = requests.delete(self.server_url + "user/" + username)
        return response.json()
    
    def post_user_profile(self, profile):
        response = requests.post(self.server_url + "user/", json=profile)
        print(response.content)
        return response.json()

# Test
server = ServerConnection()
print(server.is_connected)
print(server.post_user_profile(profile = {"username": "test", "name": "test", "extraversion": 3}))
print(server.get_user_profile(usename = "test"))
print(server.delete_user_profile(username = "test"))
print(server.get_user_profile(username = "test"))