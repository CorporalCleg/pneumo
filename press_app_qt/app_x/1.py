import requests
import json

# Отправляем GET-запрос для получения JSON-данных
response = requests.get(url="http://example.com/api")

# Проверяем статус код ответа
if response.status_code == 200:
    # Получаем JSON-данные из ответа
    json_data = response.json()

    # Выводим полученные данные
    print(json_data)
else:
    print("Ошибка при получении данных")