# Course_work

# Інструкція для запуску C++ проєкту

Щоб успішно запустити ваш проєкт на C++, дотримуйтесь наступних кроків:

---

## 1. Попередні вимоги
Перед тим як почати, переконайтеся, що у вас встановлено:
- **Компилятор C++** (наприклад, GCC, Clang або Microsoft Visual C++).
- **CMake**
- **Git**
- **Docker**
- **Python 3.12**

---

## 2. Кроки для збірки та запуску Серверу

### a) Клонування репозиторію (за потреби)
Якщо проєкт зберігається у віддаленому репозиторії Git, виконайте:
```bash
git clone <URL_репозиторію>
cd <імя_папки_проєкту>
Завантажити докер
в терміналі прописати
docker build -t cpp-course-project .
docker run --name <назва контейнеру> -p <порт>:<порт> cpp-course-project <порт> <кількість потоків для средпулу> <збудувати з існуючими файлами?>
```
## 2. Кроки для збірки та запуску Клієнту на пітоні
Unix
```bash 
$ cd python_client_src/
python -m venv .venv
source .venv/bin/acitvate
pip install -r requirements.txt
# Usage
python main.py
```

Windows
```bash 
$ cd python_client_src/
python -m venv .venv
.venv/bin/acitvate
pip install -r requirements.txt
# Usage
python main.py
```