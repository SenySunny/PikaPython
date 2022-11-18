
class _MQTT:
    def __init__(self, ip: str, port: int, clinetID: str,
                 username: str, password: str, version: str,
                 ca: str, keepalive: int): ...

    def __del__(self): ...

    def set_port(self, port: int) -> int:
        """Set the port of the MQTTClient."""

    def set_host(self, host_url: str) -> int:
        """Set the host_url of the MQTTClient."""

    def set_client_id(self, id: str) -> int:
        """Set the client_id of the MQTTClient."""

    def set_user_name(self, name: str) -> int:
        """Set the name of the MQTTClient."""

    def set_password(self, password: str) -> int:
        """Set the password of the MQTTClient."""

    def connect(self) -> int:
        """connect to the mqtt-server."""

    def disconnect(self) -> int:
        """disconnect to the mqtt-server."""
