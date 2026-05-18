-- Initial Schema

CREATE TABLE IF NOT EXISTS schema_version (
    version INTEGER PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS levels (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    unlocked BOOLEAN DEFAULT 0,
    best_score INTEGER DEFAULT 0,
    tokens_collected INTEGER DEFAULT 0,
    stars INTEGER DEFAULT 0
);

CREATE TABLE IF NOT EXISTS scores (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    level_id INTEGER,
    score INTEGER,
    date TEXT,
    train_type TEXT,
    FOREIGN KEY(level_id) REFERENCES levels(id)
);

CREATE TABLE IF NOT EXISTS ghost_runs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    level_id INTEGER,
    mode TEXT,
    input_data BLOB,
    timestamp TEXT,
    FOREIGN KEY(level_id) REFERENCES levels(id)
);

CREATE TABLE IF NOT EXISTS settings (
    key TEXT PRIMARY KEY,
    value TEXT
);

CREATE TABLE IF NOT EXISTS achievements (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    unlocked BOOLEAN DEFAULT 0,
    date TEXT
);
