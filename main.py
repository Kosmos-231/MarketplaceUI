import sys
import random
import sqlite3
import time
import pandas as pd # Импортируем pandas для работы с Excel

def init_db():
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

def save_demo_data(product_id):
    tech_names = ["Умные часы", "Беспроводные наушники", "Игровой монитор", "Механическая клавиатура", "Вертикальная мышь"]
    brands = ["Xiaomi", "Samsung", "Logitech", "Apple", "Huawei", "Asus"]
    
    random.seed(int(product_id))
    name = random.choice(tech_names) + f" (Модель {product_id})"
    brand = random.choice(brands)
    
    price_generator = random.Random(time.time_ns())
    price = price_generator.randint(15, 450) * 100 
    
    conn = sqlite3.connect("marketplace.db")
    cursor = conn.cursor()
    cursor.execute(
        "INSERT INTO products (id, name, brand, price) VALUES (?, ?, ?, ?)",
        (product_id, name, brand, price)
    )
    conn.commit()
    conn.close()
    print(f"[Демо] Записано в базу: {name} за {price} руб.", flush=True)

def export_to_excel(product_id):
    """Вытаскивает всю историю товара из базы данных и сохраняет в Excel-файл"""
    try:
        conn = sqlite3.connect("marketplace.db")
        
        # SQL-запрос для получения истории конкретного товара
        query = f"SELECT id AS 'ID Товара', name AS 'Название', brand AS 'Бренд', price AS 'Цена (руб.)', timestamp AS 'Дата замера' FROM products WHERE id = '{product_id}' ORDER BY timestamp ASC"
        
        # Pandas одной командой считывает SQL и превращает его в таблицу DataFrame
        df = pd.read_sql_query(query, conn)
        conn.close()
        
        if df.empty:
            print(f"[Экспорт] Ошибка: Нет данных для ID {product_id}", flush=True)
            return

        # Имя файла для сохранения
        filename = f"history_product_{product_id}.xlsx"
        
        # Сохраняем в красивый Excel-файл без индекса строк
        df.to_excel(filename, index=False)
        print(f"[Экспорт] Успешно сохранен файл: {filename}", flush=True)
        
    except Exception as e:
        print(f"[Экспорт] Ошибка при выгрузке: {e}", flush=True)

if __name__ == "__main__":
    init_db()
    
    if len(sys.argv) > 1:
        # Проверяем, если C++ вызвал команду экспорта: python main.py <ID> --export
        if len(sys.argv) > 2 and sys.argv[2] == "--export":
            export_to_excel(sys.argv[1])
        else:
            # Обычный парсинг: python main.py <ID>
            save_demo_data(sys.argv[1])
    else:
        save_demo_data("444333")