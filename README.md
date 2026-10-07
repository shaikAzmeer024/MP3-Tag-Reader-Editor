# 🎵 MP3 Tag Reader and Editor (ID3v2)

> A lightweight C-based command-line application to read and edit MP3 metadata using the **ID3v2.3 standard**. This project demonstrates **low-level file handling, binary data processing, and metadata manipulation without using external libraries**.

---

## 🚀 Features

### 👁️ View MP3 Metadata

- **Title**
- **Artist**
- **Album**
- **Year**
- **Genre (Content)**
- **Comments**

### ✏️ Edit MP3 Tags

| Option | Description |
|---|---|
| `-t` | Title |
| `-a` | Artist |
| `-A` | Album |
| `-y` | Year |
| `-m` | Genre |
| `-c` | Comments |

---

## 🛠️ Technologies Used

**Language:** C

### Concepts Used

- File handling (binary mode)
- Structures
- Command-line arguments
- Dynamic memory allocation
- Endianness conversion

---

## ▶️ Usage

### 👁️ View Metadata

```bash
./a.out -v sample.mp3
```

### ✏️ Edit Metadata

**Edit Title**
```bash
./a.out -e -t "New Title" sample.mp3
```

**Edit Artist**
```bash
./a.out -e -a "Artist Name" sample.mp3
```

**Edit Album**
```bash
./a.out -e -A "Album Name" sample.mp3
```

**Edit Year**
```bash
./a.out -e -y "2025" sample.mp3
```

**Edit Genre**
```bash
./a.out -e -m "Genre" sample.mp3
```

**Edit Comments**
```bash
./a.out -e -c "Comments" sample.mp3
```

---

## ⚙️ How It Works

### 🔍 Validation

Validates MP3 file and checks for **ID3v2.3 header**.

### 📖 Reading

Reads metadata from predefined frames **(TIT2, TPE1, etc.)**.

### ✏️ Editing

For editing:

1. Creates a temporary file
2. Updates selected tag
3. Copies remaining data
4. Replaces original file

---

## ✅ Advantages

- Simple and efficient
- No external libraries required
- Direct binary-level metadata manipulation

---

## ⚠️ Limitations

- Supports only ID3v2.3
- Limited to basic tags
- CLI-based (no GUI)

---

## 📌 Learning Outcomes

- Understanding MP3 file structure
- Working with binary files in C
- Implementing real-world file manipulation tools

---

## 🎯 Conclusion

This project showcases how low-level programming in C can be used to read and modify MP3 metadata efficiently. It strengthens understanding of **binary file handling, ID3 tag structure, and real-world application development without relying on external libraries**.
