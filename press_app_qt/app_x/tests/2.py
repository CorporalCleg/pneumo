import requests
import json

# Создаем словарь с данными
data = {
    "name": "John",
    "age": 30,
    "city": "New York"
}

# Преобразуем словарь в JSON-строку
json_data = json.dumps(data)

# Отправляем POST-запрос с JSON-данными
response = requests.post(url="http://example.com/api", json=json_data)

# Проверяем статус код ответа
if response.status_code == 200:
    print("Данные успешно отправлены")
else:
    print("Ошибка при отправке данных")