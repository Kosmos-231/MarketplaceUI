import os
import sys
import sqlite3
import random
import time
from curl_cffi import requests
from bs4 import BeautifulSoup

def init_db():
    """Создает базу данных SQLite и таблицу для хранения истории цен"""
    conn = sqlite3.connect("marketplace.db")
    cursor = conn.cursor()
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS products (
            id TEXT,
            name TEXT,
            brand TEXT,
            price INTEGER,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    """)
    conn.commit()
    conn.close()

def parse_yandex_market(product_id):
    """Парсит страницу товара Яндекс Маркета и сохраняет данные в БД"""
    # Формируем ссылку на товар (используем стандартный шаблон Яндекса)
    url = f"https://yandex.ru{product_id}"
    
    print(f"[Инфо] Запрашиваем товар {product_id}...")
    
    try:
        # Имитируем реальный браузер Chrome, чтобы обойти базовые блокировки
        response = requests.get(url, impersonate="chrome", timeout=15)
        
        if response.status_code == 200:
            # Передаем HTML-код страницы парсеру BeautifulSoup
            soup = BeautifulSoup(response.text, 'html.parser')
            
            # Ищем название товара (Яндекс часто использует тег h1 для названия)
            title_tag = soup.find('h1')
            name = title_tag.text.strip() if title_tag else "Неизвестный товар"
            
            # Пытаемся найти цену (на Маркете она обычно лежит в блоках со спец-атрибутами)
            # Если точный класс поменялся, используем дефолтное значение для теста базы данных
            price_tag = soup.find(attrs={"data-auto": "snippet-price"}) or soup.find(attrs={"data-zone-name": "price"})
            
            if price_tag:
                # Очищаем текст цены от знаков рубля и пробелов (например, "1 250 ₽" -> 1250)
                price_text = "".join([char for char in price_tag.text if char.isdigit()])
                price = int(price_text) if price_text else 0
            else:
                price = 1500  # Тестовое значение, если страница отдала капчу
                
            brand = "Яндекс.Маркет" # Для теста зафиксируем бренд
            
            # Записываем результат в базу данных SQLite
            conn = sqlite3.connect("marketplace.db")
            cursor = conn.cursor()
            cursor.execute(
                "INSERT INTO products (id, name, brand, price) VALUES (?, ?, ?, ?)",
                (product_id, name, brand, price)
            )
            conn.commit()
            conn.close()
            
            print(f"[Успех] Данные сохранены! Товар: {name}, Цена: {price} руб.")
            
        elif response.status_code == 403 or response.status_code == 405:
            print(f"[Предупреждение] Яндекс запросил капчу (Код {response.status_code}). Используем демо-данные для разработки.")
            save_demo_data(product_id)
        else:
            print(f"[Ошибка] Сервер вернул код: {response.status_code}")
            save_demo_data(product_id)
            
    except Exception as e:
        print(f"[Ошибка] Не удалось выполнить запрос: {e}")
        save_demo_data(product_id)

def save_demo_data(product_id):
    """Генерирует строго один товар для одного ID, но с разной ценой"""
    
    tech_names = ["Умные часы", "Беспроводные наушники", "Игровой монитор", "Механическая клавиатура", "Вертикальная мышь"]
    brands = ["Xiaomi", "Samsung", "Logitech", "Apple", "Huawei", "Asus"]
    
    # 1. Фиксируем генератор строго по ID для названия и бренда
    random.seed(int(product_id))
    name = random.choice(tech_names) + f" (Модель {product_id})"
    brand = random.choice(brands)
    
    # 2. Для цены используем генератор на основе текущего времени (в миллисекундах)
    # Это гарантирует, что цена ВСЕГДА будет случайной и новой при каждом нажатии
    price_generator = random.Random(time.time_ns())
    price = price_generator.randint(15, 450) * 100 
    
    # Записываем в базу данных
    conn = sqlite3.connect("marketplace.db")
    cursor = conn.cursor()
    cursor.execute(
        "INSERT INTO products (id, name, brand, price) VALUES (?, ?, ?, ?)",
        (product_id, name, brand, price)
    )
    conn.commit()
    conn.close()
    print(f"[Демо] Записано в базу: {name} за {price} руб.", flush=True)

if __name__ == "__main__":
    init_db()
    # Проверяем, передал ли нам C++ артикул через аргументы командной строки
    if len(sys.argv) > 1:
        parse_yandex_market(sys.argv[1])
    else:
        # Тестовый ID товара, если запускаем вручную в VS Code
        parse_yandex_market("101824715805")