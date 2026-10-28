# What2Watch

What2Watch uses a Crow C++ server to host both the site and its JSON API. The page and API use the same origin, so they need no frontend API URL, CORS rule, `.env` file, or external API key.

## Run locally in VS Code

1. Open this folder in VS Code.
2. Choose **Terminal → Run Task… → What2Watch: Build and run**. The task builds the Crow server and starts it in the integrated terminal.
3. Open <http://localhost:18080> in your browser.

You can also run `.\run-local.ps1` from a PowerShell terminal opened in this folder. Keep the terminal open while using the site; press **Ctrl+C** there to stop the server. The script tries available MinGW compilers and builds `what2watch_server_local.exe` before launching it.

The default port is `18080`. If that port is occupied, set `WHAT2WATCH_PORT` before starting. For example:

```powershell
$env:WHAT2WATCH_PORT = "18081"
.\run-local.ps1
```

Then open <http://localhost:18081>.

## Recommendation API

`GET /api/recommendations?name=Sam&mood=happy&preference=Movie` returns JSON with `name`, `mood`, `preference`, and `items`. Each item includes `title`, `genre`, `rating`, `type`, `mood`, `poster`, `durationMinutes`, and `seasons`; the field that does not apply is `null`. The backend validates the name, mood, and content preference and returns HTTP 400 with a JSON error when input is invalid.

The C++ catalog is a curated sample dataset spanning English, Hindi, Korean, Japanese, German, and Spanish titles, with release year, genre, rating, and runtime or season count. It currently contains 53 distinct movies and series. **Sad** maps to emotional titles, **Relaxed** to the comedy catalog, and **Excited** maps to thrillers; **Adventurous** has its own adventure selection. Ratings and catalog metadata are sample data and are not refreshed from a live provider.

Poster paths are stored in the backend catalog and returned in the `poster` field. TMDB image paths use the image CDN format (`https://image.tmdb.org/t/p/w500/{file_path}`); these images need an internet connection and do not require a TMDB API key. Some sample titles have no poster path. In those cases, or if an image request fails, the browser displays the bundled `/poster-fallback.svg` image. A provider adapter can later fill missing metadata and poster paths; keep any provider key in backend environment configuration, never in `script.js`.
