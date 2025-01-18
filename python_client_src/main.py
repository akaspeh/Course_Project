# noinspection PyInterpreter
import string
import os
import time
import threading
import random
import matplotlib.pyplot as plt
import matplotlib
import numpy as np

matplotlib.use("TkAgg")
from Client import Client, RequestType

SERVER_ADDRESS = "127.0.0.1"
SERVER_PORT = 8080

# Metrics storage
performance_metrics = []
fail_connections = []

def get_all_files(directory):
    """Recursively gets a list of all files in the directory."""
    file_paths = []
    for root, _, files in os.walk(directory):
        for file in files:
            file_paths.append(os.path.join(root, file))
    return file_paths

def client_worker(client_id, files, num_of_files):
    """Function to simulate a client performing actions."""
    global fail_connections
    client = Client(SERVER_ADDRESS, SERVER_PORT)
    try:
        for _ in range(num_of_files):
            # Choose a random file
            file_path = random.choice(files)
            file_name = os.path.basename(file_path)

            # Read file content
            with open(file_path, 'r') as f:
                content = f.read()

            # Upload file
            try:
                client.connect()
                upload_start = time.time()
                client.upload_file(file_name, content)
                upload_end = time.time()
                performance_metrics.append(("upload", upload_end - upload_start))
                client.close_connection()
            except Exception as e:
                fail_connections.append((client_id, "upload"))
                print(f"Client {client_id}: Upload failed - {e}")

            # Search for a term (if content is not empty)
            if content.strip():
                try:
                    search_term = random.choice(content.split())
                    client.connect()
                    search_start = time.time()
                    client.search_file(search_term)
                    search_end = time.time()
                    performance_metrics.append(("search", search_end - search_start))
                    client.close_connection()
                except Exception as e:
                    fail_connections.append((client_id, "search"))
                    print(f"Client {client_id}: Search failed - {e}")

            # Delete file
            try:
                client.connect()
                delete_start = time.time()
                client.delete_file(file_name)
                delete_end = time.time()
                performance_metrics.append(("delete", delete_end - delete_start))
                client.close_connection()
            except Exception as e:
                fail_connections.append((client_id, "delete"))
                print(f"Client {client_id}: Delete failed - {e}")

    except Exception as e:
        print(f"Client {client_id}: Error occurred - {e}")
    finally:
        client.close_connection()


def run_test(client_counts, directory, num_of_files):
    """Run tests for varying numbers of clients."""
    global performance_metrics, fail_connections
    files = get_all_files(directory)
    if not files:
        print(f"Error: No files found in the specified directory: {directory}")
        return []

    results = []

    for num_clients in client_counts:
        print(f"Testing with {num_clients} clients...")
        threads = []
        performance_metrics = []
        fail_connections = []
        test_start = time.time()
        files_per_client = num_of_files // num_clients  # Распределяем файлы равномерно
        # Launch clients
        for client_id in range(num_clients):
            thread = threading.Thread(target=client_worker, args=(client_id, files, files_per_client))
            threads.append(thread)
            thread.start()

        # Wait for all clients to complete
        for thread in threads:
            thread.join()

        test_end = time.time()
        total_duration = test_end - test_start  # время, затраченное на выполнение всех клиентов

        # Collect results
        if performance_metrics:
            total_time = sum(time for _, time in performance_metrics)
            avg_time = total_time / len(performance_metrics)
        else:
            total_time = 0  # Если нет данных, считаем, что времени не было
            avg_time = 0  # И соответственно среднее тоже 0

        # Добавляем результаты в список, включая время выполнения всех файлов
        results.append((num_clients, total_time, avg_time, total_duration, len(fail_connections)))

    return results

def plot_results(results):
    """Plot the average time for different client counts."""
    # Извлекаем данные для графика
    client_counts = [r[0] for r in results]  # Количество клиентов
    avg_times = [r[2] for r in results]  # Среднее время выполнения
    fail_counts = [r[4] for r in results]  # Количество ошибок соединений

    # График для среднего времени
    plt.figure(figsize=(10, 5))
    plt.subplot(1, 2, 1)
    plt.plot(client_counts, avg_times, marker='o', color='b', label='Avg Time (s)')
    plt.xlabel('Number of Clients')
    plt.ylabel('Average Time (s)')
    plt.title('Average Time vs Number of Clients')
    plt.grid(True)

    # Гистограмма для ошибок соединений
    plt.subplot(1, 2, 2)
    plt.bar(client_counts, fail_counts, color='r', label='Fail Connections')
    plt.xlabel('Number of Clients')
    plt.ylabel('Fail Connections')
    plt.title('Fail Connections vs Number of Clients')
    plt.grid(True)

    plt.tight_layout()
    plt.show()


def plot_total_duration(results):
    """Plot the total duration for different client counts."""
    # Извлекаем данные для графика
    client_counts = [r[0] for r in results]  # Количество клиентов
    total_durations = [r[3] for r in results]  # Общее время выполнения всех клиентов

    # График для времени выполнения всех файлов
    plt.figure(figsize=(10, 5))
    plt.plot(client_counts, total_durations, marker='o', color='g', label='Total Duration (s)')
    plt.xlabel('Number of Clients')
    plt.ylabel('Total Duration (s)')
    plt.title('Total Duration vs Number of Clients')
    plt.grid(True)

    plt.tight_layout()
    plt.show()

def plot_histogram(results):
    """
    Построить гистограмму среднего времени операции и количества ошибок соединения.

    :param results: Список кортежей (num_clients, avg_time, fail_count)
    """
    if not results:
        print("No results to plot.")
        return

    # Извлечение данных
    client_counts = [r[0] for r in results]  # Количество клиентов
    times = [r[3] for r in results]  # Среднее время операций

    # Настройка столбцов
    bar_width = 0.35
    indices = np.arange(len(client_counts))  # Индексы для столбцов

    # Построение гистограммы
    fig, ax1 = plt.subplots()

    # Столбцы для среднего времени
    bars1 = ax1.bar(indices, times, bar_width, label='Avg Time (s)', color='blue')

    # Вторая ось Y для количества ошибок


    # Подписи и легенда
    ax1.set_xlabel('Number of Clients')
    ax1.set_ylabel('Time (s)', color='blue')


    ax1.set_xticks(indices + bar_width / 2)
    ax1.set_xticklabels(client_counts)
    ax1.set_title('Performance by Number of Clients')

    # Отображение графика
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    # Specify test parameters
    CLIENT_COUNTS = [1, 5, 10, 20, 50]  # Different numbers of clients
    TEST_DIRECTORY = "./Files"  # Directory containing test files

    # Run the tests and plot results
    results = run_test(CLIENT_COUNTS, TEST_DIRECTORY, 1000)
    plot_results(results)  # Графики для времени выполнения и ошибок соединений
    plot_total_duration(results)
    plot_histogram(results)



# if __name__ == "__main__":
#     server_address = '127.0.0.1'  # Replace with the actual server address
#     server_port = 18080  # Replace with the actual server port
#
#     # Create client instance
#     client = Client(server_address, server_port)
#
#     # Connect to the server
#
#     # Test file upload
#     file_name = "test_file.txt"
#     content = "This is a test file."
#     print(f"Uploading file: {file_name}")
#     client.connect()
#     print(client.upload_file(file_name, content))
#     client.close_connection()
#
#     # Test file search
#     search_term = "test"
#     print(f"Searching for files with term: {search_term}")
#     client.connect()
#     print(client.search_file(search_term))
#     client.close_connection()
#
#     # Test file delete
#     print(f"Deleting file: {file_name}")
#     client.connect()
#     print(client.delete_file(file_name))
#     client.close_connection()



