import socket
import struct
from enum import Enum

class RequestType(Enum):
    FUPLOAD = 0
    FSEARCH = 1
    FDELETE = 2


class Client:
    def __init__(self, server_address, server_port):
        self.server_address = server_address
        self.server_port = server_port
        self.client_socket = None

    def connect(self):
        try:
            # Создаем новый сокет и подключаемся
            self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.client_socket.connect((self.server_address, self.server_port))
        except Exception as e:
            print(f"Error connecting to server: {e}")

    def close_connection(self):
        try:
            if self.client_socket:
                self.client_socket.close()
                self.client_socket = None  # Сделайте сокет None, чтобы предотвратить повторное использование
        except Exception as e:
            print(f"Error closing connection: {e}")

    def send_request(self, request_type, data):
        try:
            # Преобразуем данные в байты, если это строка
            if isinstance(data, str):
                data = data.encode('utf-8')

            # Добавляем тип запроса в начало данных
            request_type_value = request_type.value
            data_with_type = struct.pack('B', request_type_value) + data

            # Определяем длину данных
            data_length = len(data_with_type)
            print(data_length)


            # Отправляем длину данных (4 байта, unsigned int)
            self.client_socket.send(struct.pack('I', data_length))
            # Отправляем данные
            self.client_socket.send(data_with_type)

            # Получаем длину ответа
            response_length_data = self.client_socket.recv(4)
            if not response_length_data or len(response_length_data) != 4:
                print("Failed to receive response length.")
                return

            response_length = struct.unpack('I', response_length_data)[0]
            print(response_length)
            # Получаем сам ответ
            response_data = b""
            while len(response_data) < response_length:
                chunk = self.client_socket.recv(response_length - len(response_data))
                if not chunk:
                    print("Failed to receive full response data.")
                    return None
                response_data += chunk

            response = response_data.decode('utf-8')
            print(f"Received response: {response}")
            return response
        except Exception as e:
            print(f"Error sending request: {e}")
            return "Error"

    def upload_file(self, file_name, content):
        request_type = RequestType.FUPLOAD
        data = f"{file_name}\n{content}"
        self.send_request(request_type, data)

    def delete_file(self, file_name):
        request_type = RequestType.FDELETE
        data = file_name
        self.send_request(request_type, data)

    def search_file(self, search_term):
        request_type = RequestType.FSEARCH
        data = search_term
        self.send_request(request_type, data)