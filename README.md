# Student Database Management System (SDMS)

A simple Student Database Management System combining a C++ backend and a static frontend.

**Project Overview**
- **Description:**: Minimal student database demo with a C++ program (`backend.cpp`) that reads/writes `data.json`, and a web frontend (`frontend/`) (HTML/JS/CSS) for interaction and display.
- **Language/Tech:**: C++17, header-only `nlohmann/json` (included under `include/nlohmann/json.hpp`), plain HTML/CSS/JavaScript for the frontend.

**Quick Start**
- **Open Project:**: Open the project folder at `c:\Users\Desktop\SDMS` in your editor.

**Prerequisites**
- **C++ Compiler:**: `g++` (MinGW / GCC) or MSVC (`cl`).
- **Python (optional):**: `python` for serving the frontend via a simple HTTP server (recommended for correct file/URL behavior).
- **Web Browser:**: Modern browser (Chrome, Edge, Firefox).

**Build & Run — Backend (PowerShell)**
- **Compile with g++:**

```powershell
cd C:\Users\anujr\OneDrive\Desktop\SDMS
g++ -std=c++17 -I include -o backend.exe backend.cpp
```

- **Compile with MSVC (Developer Command Prompt / cl):**

```powershell
cd C:\Users\Desktop\SDMS
cl /EHsc /I include backend.cpp
```

- **Run backend:**

```powershell
.\backend.exe
```

Notes:
- The project uses the header-only `nlohmann/json` library already present at `include/nlohmann/json.hpp` so no extra package installation is required for JSON support.
- The backend reads/writes `data.json` in the project root. Ensure the process has write permissions to that file.

**Run — Frontend**
- Easiest: open `frontend/index.html` directly in a browser.
- Recommended (serves files over HTTP):

```powershell
cd C:\Users\Desktop\SDMS\frontend
python -m http.server 8000
```

Then open `http://localhost:8000/index.html` in your browser.

**Project Structure**
- `backend.cpp`: C++ backend program that manipulates `data.json`.
- `data.json`: The JSON data store for student records.
- `frontend/`:
  - `index.html`: Frontend UI.
  - `app.js`: Frontend JavaScript logic.
  - `style.css`: Frontend styles.
  - `run.txt`: (auxiliary file; inspect for notes)
- `include/nlohmann/json.hpp`: Header-only JSON library used by `backend.cpp`.

**Data format**
- `data.json` contains student records in JSON format. Open it to inspect or modify sample entries. The backend and frontend expect JSON objects/arrays — check `backend.cpp` and `frontend/app.js` for exact field names.

- 
**Screenshot 📸**
  <img width="1918" height="908" alt="image" src="https://github.com/user-attachments/assets/9af9c220-c99b-49c7-8132-92fbb65f572f" />
  <img width="1611" height="848" alt="image" src="https://github.com/user-attachments/assets/e4d16852-e9ac-49eb-b17f-fa984cfd1a2a" />



**Troubleshooting**
- **Compiler errors about headers:**: Ensure the `-I include` flag is used so the compiler finds `nlohmann/json.hpp`.
- **Frontend fetch/network errors:**: Serving via `python -m http.server` avoids CORS/file:// restrictions. If opening the file directly causes issues, use the HTTP server.
- **Permission errors writing `data.json`:**: Run the backend from a directory where you have write access, or run your terminal as an elevated user if necessary.

**Contributing**
- Improvements: Add detailed build scripts, a small REST API, or a database-backed store.
- Please open an issue or submit a pull request describing changes.

**License & Contact**
- No license file included. Add one if you intend to publish.
- For questions, open an issue in the repo or contact the maintainer.



