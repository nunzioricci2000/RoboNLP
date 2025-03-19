import requests
from model.server_response import ServerResponse
from model.user_profile import UserProfile
import config

class ServerConnection():
    instance = None
    is_connected = False
    server_url = f"http://{config.server_ip}:{config.server_port}/"

    def __new__(cls): # Singleton pattern
        if ServerConnection.instance is None:
            ServerConnection.instance = object.__new__(cls)
        return ServerConnection.instance

    def __init__(self):
        if not self.is_connected:
            try:
                response = requests.get(self.server_url)
                if response.status_code == 200:
                    self.is_connected = True
            except requests.exceptions.ConnectionError:
                self.is_connected = False
                print("Server not connected")

    def get_user_profile(self, username: str) -> ServerResponse:
        response = requests.get(f"{self.server_url}user/{username}")
        if response.status_code == 200:
            ret = ServerResponse.from_profile(response.json())
        elif response.status_code == 404:
            ret = ServerResponse.error("Not found")
        else:
            ret = ServerResponse.error("Error")
        return ret
    
    def get_user_profile_field(self, username: str, field: str) -> ServerResponse:
        response = requests.get(f"{self.server_url}user/{username}/{field}")
        if response.status_code == 200:
            ret = ServerResponse.from_profile(response.json())
        elif response.status_code == 404:
            ret = ServerResponse.error("Not found")
        else:
            ret = ServerResponse.error("Error")
        return ret
    
    def delete_user_profile(self, username: str) -> ServerResponse:
        response = requests.delete(f"{self.server_url}user/{username}")
        if response.status_code == 200:
            ret = ServerResponse.success("User by the username " + username + "deleted")
        else:
            ret = ServerResponse.error("Error. DELETE operation failed for user " + username)
        return ret
    
    def delete_user_profile_field(self, username: str, field: str) -> ServerResponse:
        response = requests.delete(f"{self.server_url}user/{username}/{field}")
        if response.status_code == 200:
            ret = ServerResponse.success("Filed " + field + " for the user " + username + " deleted")
        else:
            ret = ServerResponse.error("Error. DELETE " + field + " operation failed for user " + username)
        return ret
    
    def post_user_profile(self, username: str,  profile: UserProfile) -> ServerResponse:
        json = profile.to_json()
        json["username"] = username
        response = requests.post(f"{self.server_url}user/", json=json)
        if response.status_code == 200:
            ret = ServerResponse.success("User posted succesfully")
        else:
            ret = ServerResponse.error("Error. POST operation failed.")
        return ret
    
    def put_user_profile(self, username: str, to_update: UserProfile) -> ServerResponse:
        json = to_update.to_json()
        response = requests.put(f"{self.server_url}user/{username}", json=json)
        if response.status_code == 200:
            ret = ServerResponse.success("User updated succesfully")
        else:
            ret = ServerResponse.error("Error. PUT operation failed")
        return ret
    
    def post_user_profile_facts(self, username: str, facts: str) -> ServerResponse:
        json = { "fact": facts }
        response = requests.post(f"{self.server_url}user/{username}/facts", json=json)
        if response.status_code == 200:
            ret = ServerResponse.success("User fact posted successfully")
        else:
            ret = ServerResponse.error("Error. POST operation failed")
        return ret


if __name__ == "__main__":
    # Test
    server = ServerConnection()
    assert server.is_connected, "Could not connect to server"
    profile = UserProfile(name="test", extraversion=3)
    post_response = server.post_user_profile(username="test", profile=profile)
    print(f"GET: {server.get_user_profile(username='test').to_string()}")
    print(f"GET field: {server.get_user_profile_field(username='test', field='extraversion').to_string()}")
    print(f"PUT: {server.put_user_profile(username='test', to_update=UserProfile(agreeableness=5)).to_string()}")
    print(f"GET: {server.get_user_profile(username='test').user_profile.to_string()}")
    print(f"DELETE field: {server.delete_user_profile_field(username='test', field='agreeableness').to_string()}")
    print(f"GET: {server.get_user_profile(username='test').to_string()}")
    print(f"POST facts: {server.post_user_profile_facts(username='test', facts='first fact').to_string()}")
    print(f"POST facts: {server.post_user_profile_facts(username='test', facts='second fact\nthird fact').to_string()}")
    print(f"GET: {server.get_user_profile(username='test').to_string()}")
    print(f"DELETE: {server.delete_user_profile(username='test').to_string()}")
    print(f"GET: {server.get_user_profile(username='test').to_string()}")