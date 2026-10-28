#include "crow_all.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct Media {
    std::string title;
    std::string genre;
    double rating;
    std::string type;
    std::string mood;
    std::string poster;
    int duration = 0;
    int seasons = 0;
    int year = 0;
    std::string language = "English";
};

const std::string posterBaseUrl = "https://image.tmdb.org/t/p/w500";

const std::vector<Media> catalog = {
    // Starter catalog retained from the existing C++ console app; replace this source with a provider adapter when a real API is configured.
    {"3 Idiots", "Comedy", 8.4, "Movie", "Happy", "https://image.tmdb.org/t/p/w500/66A9MqXOyVFCssoloscw79z8Tew.jpg", 171, 0, 2009, "Hindi"},
    {"Zindagi Na Milegi Dobara", "Comedy", 8.2, "Movie", "Happy", "/j8sE9Ogeifh5Pk89eo5zRgOwGaX.jpg", 155, 0, 2011, "Hindi"},
    {"Panchayat", "Comedy", 8.9, "Series", "Happy", "/hFNANSq7eunt0xAExXekSesaUJG.jpg", 0, 4, 2020, "Hindi"},
    {"Brooklyn Nine-Nine", "Comedy", 8.4, "Series", "Happy", "/rFTibDtQ8IlvwuVcJJSeBv0yaoo.jpg", 0, 8, 2013, "English"},
    {"Jab We Met", "Romance", 7.9, "Movie", "Romantic", "/iqiH6vnoEgjAypKsuIScN7JlE2j.jpg", 138, 0, 2007, "Hindi"},
    {"The Notebook", "Romance", 7.8, "Movie", "Romantic", "/rNzQyW4f8B8cQeg7Dgj3n6eT5k9.jpg", 123, 0, 2004, "English"},
    {"Crash Landing on You", "Romance", 8.7, "Series", "Romantic", "/fgBNLPr6mC8pxuR79ENAJY4nBmj.jpg", 0, 1, 2019, "Korean"},
    {"Modern Love", "Romance", 7.9, "Series", "Romantic", "/cP9CJ9nvPT3cnnBSlGFgwq7odIR.jpg", 0, 2, 2019, "English"},
    {"Inception", "Thriller", 8.8, "Movie", "Thriller", "https://image.tmdb.org/t/p/w500/oYuLEt3zVCKq57qu2F8dT7NIa6f.jpg", 148, 0, 2010, "English"},
    {"Drishyam", "Thriller", 8.2, "Movie", "Thriller", "/gIClWRv5OSe8rl5Koi0AeUcCZ9Z.jpg", 163, 0, 2015, "Hindi"},
    {"Stranger Things", "Sci-Fi", 8.6, "Series", "Thriller", "https://image.tmdb.org/t/p/w500/49WJfeN0moxb9IPfGn8AIqMGskD.jpg", 0, 5, 2016, "English"},
    {"Money Heist", "Thriller", 8.2, "Series", "Thriller", "/dmnwJGVergOFWCiLbPsQTyyzePw.jpg", 0, 5, 2017, "Spanish"},
    {"Shutter Island", "Mystery", 8.2, "Movie", "Mystery", "/qnWJFFkRv61e030Yc4xG8oVZUqG.jpg", 138, 0, 2010, "English"},
    {"Gone Girl", "Mystery", 8.1, "Movie", "Mystery", "/vaWOHX5VEFLld5X6xli5TzfWJt6.jpg", 149, 0, 2014, "English"},
    {"Wednesday", "Mystery", 8.0, "Series", "Mystery", "https://image.tmdb.org/t/p/w500/9PFonBhy4cQy7Jz20NpMygczOkv.jpg", 0, 2, 2022, "English"},
    {"Sherlock", "Mystery", 9.1, "Series", "Mystery", "https://image.tmdb.org/t/p/w500/7WTsnHkbA0FaG6R9twfFde0I9hl.jpg", 0, 4, 2010, "English"},
    {"Interstellar", "Sci-Fi", 8.7, "Movie", "Emotional", "https://image.tmdb.org/t/p/w500/gEU2QniE6E77NI6lCU6MxlNBvIx.jpg", 169, 0, 2014, "English"},
    {"Taare Zameen Par", "Drama", 8.3, "Movie", "Emotional", "/gpTLhQI0yPiBTBdbGrDKZmiJ2bl.jpg", 165, 0, 2007, "Hindi"},
    {"This Is Us", "Drama", 8.7, "Series", "Emotional", "/e8GCDxaLEPygQDyHwmxDMlHp0w7.jpg", 0, 6, 2016, "English"},
    {"Kota Factory", "Drama", 9.0, "Series", "Emotional", "/8co48kYbJObx8wog6KSwWdOLgrf.jpg", 0, 3, 2019, "Hindi"},
    {"Toy Story", "Animation · Adventure", 8.3, "Movie", "Happy", "/uXDfjJbdP4ijW5hWSBrPrlKpxab.jpg", 81, 0, 1995, "English"},
    {"Paddington 2", "Comedy · Family", 7.8, "Movie", "Happy", "/gvVgMUoDPYHORiDMkpdzq98tFin.jpg", 104, 0, 2017, "English"},
    {"The Grand Budapest Hotel", "Comedy · Adventure", 8.1, "Movie", "Happy", "/eWdyYQreja6JGCzqHWXpWHDrrPo.jpg", 100, 0, 2014, "English"},
    {"The Office (US)", "Comedy", 9.0, "Series", "Happy", "/qWnJzyZhyy74gjpSjIXWmuk0ifX.jpg", 0, 9, 2005, "English"},
    {"Friends", "Comedy · Romance", 8.9, "Series", "Happy", "/2koX1xLkpTQM4IZebYvKysFW1Nh.jpg", 0, 10, 1994, "English"},
    {"La La Land", "Romance · Musical", 8.0, "Movie", "Romantic", "/uDO8zWDhfWwoFdKS4fzkUJt0Rf0.jpg", 128, 0, 2016, "English"},
    {"Pride & Prejudice", "Romance · Drama", 7.8, "Movie", "Romantic", "/o8UhmEbWPHmTUxP0lMuCoqNkbB3.jpg", 129, 0, 2005, "English"},
    {"Your Name", "Animation · Romance", 8.4, "Movie", "Romantic", "/q719jXXEzOoYaps6babgKnONONX.jpg", 106, 0, 2016, "Japanese"},
    {"Business Proposal", "Romance · Comedy", 8.1, "Series", "Romantic", "/2hWL4BLwNplWQw4zCVW6YXf6Knj.jpg", 0, 1, 2022, "Korean"},
    {"Normal People", "Romance · Drama", 8.4, "Series", "Romantic", "/yUjSEmePoYmlocS78f3Wh8s4zI4.jpg", 0, 1, 2020, "English"},
    {"The Shawshank Redemption", "Drama", 9.3, "Movie", "Emotional", "/9cqNxx0GxF0bflZmeSMuL5tnGzr.jpg", 142, 0, 1994, "English"},
    {"Coco", "Animation · Family", 8.4, "Movie", "Emotional", "/gGEsBPAijhVUFoiNpgZXqRVWJt2.jpg", 105, 0, 2017, "English"},
    {"Grave of the Fireflies", "Animation · War", 8.5, "Movie", "Emotional", "/fs9rvMCDCqZMr6AGciakP0Ds7og.jpg", 89, 0, 1988, "Japanese"},
    {"Reply 1988", "Comedy · Drama", 9.0, "Series", "Emotional", "/oQwspwEqzYLEvfCw4BZX2Kvpkr5.jpg", 0, 1, 2015, "Korean"},
    {"Move to Heaven", "Drama", 8.6, "Series", "Emotional", "/gUvA20jcHABFPjYMIiZl5o9tWi3.jpg", 0, 1, 2021, "Korean"},
    {"The Dark Knight", "Action · Crime", 9.0, "Movie", "Thriller", "/qJ2tW6WMUDux911r6m7haRef0WH.jpg", 152, 0, 2008, "English"},
    {"Parasite", "Thriller · Drama", 8.5, "Movie", "Thriller", "/7IiTTgloJzvGI1TAYymCfbfl3vT.jpg", 132, 0, 2019, "Korean"},
    {"Mad Max: Fury Road", "Action · Adventure", 8.1, "Movie", "Thriller", "/8tZYtuWezp8JbcsvHYO0O46tFbo.jpg", 120, 0, 2015, "English"},
    {"Knives Out", "Mystery · Comedy", 7.9, "Movie", "Mystery", "/pThyQovXQrw2m0s9x82twj48Jq4.jpg", 131, 0, 2019, "English"},
    {"Prisoners", "Crime · Thriller", 8.2, "Movie", "Mystery", "/uhviyknTT5cEQXbn6vWIqfM4vGm.jpg", 153, 0, 2013, "English"},
    {"Dark", "Sci-Fi · Mystery", 8.7, "Series", "Mystery", "/apbrbWs8M9lyOpJYU5WXrpFbk1Z.jpg", 0, 3, 2017, "German"},
    {"Only Murders in the Building", "Comedy · Mystery", 8.1, "Series", "Mystery", "/1yjFVQZuW8aofZ5Cgol8iImsVFp.jpg", 0, 4, 2021, "English"},
    {"Breaking Bad", "Crime · Drama", 9.5, "Series", "Thriller", "/ggFHVNu6YYI5L9pCfOacjizRGt.jpg", 0, 5, 2008, "English"},
    {"Squid Game", "Thriller · Drama", 8.0, "Series", "Thriller", "/dDlEmu3EZ0Pgg93K2SVNLCjCSvE.jpg", 0, 2, 2021, "Korean"},
    {"The Last of Us", "Drama · Sci-Fi", 8.6, "Series", "Thriller", "/uKvVjHNqB5VmOrdxqAt2F7J78ED.jpg", 0, 2, 2023, "English"},
    {"Chernobyl", "Drama · History", 9.3, "Series", "Emotional", "/hlLXt2tOPT6RRnjiUmoxyG1LTFi.jpg", 0, 1, 2019, "English"},
    {"The Bear", "Comedy · Drama", 8.6, "Series", "Happy", "/sHFlbKS3WLqMnp9t2ghADIJFnuQ.jpg", 0, 4, 2022, "English"},
    {"Dune: Part Two", "Sci-Fi · Adventure", 8.5, "Movie", "Adventurous", "/1pdfLvkbY9ohJlCjQH2CZjjYVvJ.jpg", 166, 0, 2024, "English"},
    {"Spider-Man: Across the Spider-Verse", "Animation · Action", 8.6, "Movie", "Adventurous", "/8Vt6mWEReuy4Of61Lnj5Xj704m8.jpg", 140, 0, 2023, "English"},
    {"Everything Everywhere All at Once", "Sci-Fi · Comedy", 7.8, "Movie", "Adventurous", "/w3LxiVYdWWRvEVdn5RYq6jIqkb1.jpg", 140, 0, 2022, "English"},
    {"Avatar: The Last Airbender", "Animation · Adventure", 9.3, "Series", "Adventurous", "/yaGt4GIutpbXHsv48tWceWg6s56.jpg", 0, 3, 2005, "English"},
    {"Arcane", "Animation · Action", 9.0, "Series", "Adventurous", "/3dCByRr3l9xWKz9Mxk66yV0wKr1.jpg", 0, 2, 2021, "English"},
    {"The Queen's Gambit", "Drama", 8.5, "Series", "Emotional", "/qIITtEn253x1x7UTVb4g288xcxI.jpg", 0, 1, 2020, "English"}
};

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

std::string posterUrl(const std::string& source) {
    if (source.empty()) return "";
    if (source.rfind(posterBaseUrl + "/", 0) == 0) return source;
    if (source.front() == '/' && source.find("..") == std::string::npos) return posterBaseUrl + source;
    return "";
}

crow::json::wvalue serialize(const Media& media) {
    crow::json::wvalue item;
    item["title"] = media.title;
    item["genre"] = media.genre;
    item["rating"] = media.rating;
    item["type"] = media.type;
    item["mood"] = media.mood;
    item["poster"] = posterUrl(media.poster);
    item["durationMinutes"] = media.type == "Movie" ? crow::json::wvalue(media.duration) : crow::json::wvalue(nullptr);
    item["seasons"] = media.type == "Series" ? crow::json::wvalue(media.seasons) : crow::json::wvalue(nullptr);
    item["year"] = media.year;
    item["language"] = media.language;
    return item;
}

crow::json::wvalue serialize(const std::vector<const Media*>& items) {
    crow::json::wvalue::list values;
    for (const Media* item : items) values.emplace_back(serialize(*item));
    return crow::json::wvalue(std::move(values));
}

std::string readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return {};
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([] {
        auto body = readFile("index.html");
        if (body.empty()) return crow::response(404, "index.html not found. Start the server from the project folder.");
        crow::response response(body);
        response.set_header("Content-Type", "text/html; charset=utf-8");
        return response;
    });

    CROW_ROUTE(app, "/style.css")([] {
        auto body = readFile("style.css");
        if (body.empty()) return crow::response(404);
        crow::response response(body);
        response.set_header("Content-Type", "text/css; charset=utf-8");
        return response;
    });

    CROW_ROUTE(app, "/script.js")([] {
        auto body = readFile("script.js");
        if (body.empty()) return crow::response(404);
        crow::response response(body);
        response.set_header("Content-Type", "application/javascript; charset=utf-8");
        return response;
    });

    CROW_ROUTE(app, "/favicon.ico")([] {
        return crow::response(204);
    });

    CROW_ROUTE(app, "/poster-fallback.svg")([] {
        const auto body = readFile("poster-fallback.svg");
        if (body.empty()) return crow::response(404);
        crow::response response(body);
        response.set_header("Content-Type", "image/svg+xml; charset=utf-8");
        response.set_header("Cache-Control", "public, max-age=3600");
        return response;
    });

    CROW_ROUTE(app, "/api/media")([] {
        std::vector<const Media*> items;
        for (const auto& item : catalog) items.push_back(&item);
        return crow::response(serialize(items));
    });

    CROW_ROUTE(app, "/api/search")([&](const crow::request& request) {
        const std::string query = lower(request.url_params.get("q") ? request.url_params.get("q") : "");
        std::vector<const Media*> items;
        for (const auto& item : catalog) {
            if (query.empty() || lower(item.title).find(query) != std::string::npos) items.push_back(&item);
        }
        return crow::response(serialize(items));
    });

    CROW_ROUTE(app, "/api/recommendations")([&](const crow::request& request) {
        const char* rawMood = request.url_params.get("mood");
        const char* rawName = request.url_params.get("name");
        const char* rawPreference = request.url_params.get("preference");
        const std::string mood = rawMood ? lower(rawMood) : "";
        std::string name = rawName ? rawName : "";
        const std::string preference = rawPreference ? lower(rawPreference) : "";
        const auto first = name.find_first_not_of(" \t\r\n");
        const auto last = name.find_last_not_of(" \t\r\n");
        name = first == std::string::npos ? "" : name.substr(first, last - first + 1);
        const std::vector<std::string> moods = {
            "happy", "romantic", "sad", "thriller", "excited", "relaxed",
            "adventurous", "mystery", "emotional"
        };
        if (name.empty() || name.size() > 80 ||
            std::find(moods.begin(), moods.end(), mood) == moods.end() ||
            (preference != "movie" && preference != "series")) {
            crow::json::wvalue error;
            error["error"] = "Provide a name (up to 80 characters), a supported mood, and a Movie or Series preference.";
            error["items"] = crow::json::wvalue::list{};
            crow::response response(400, error.dump());
            response.set_header("Content-Type", "application/json; charset=utf-8");
            return response;
        }
        std::string catalogMood = mood;
        if (mood == "sad") catalogMood = "emotional";
        else if (mood == "relaxed") catalogMood = "happy";
        else if (mood == "excited") catalogMood = "thriller";
        std::vector<const Media*> items;
        for (const auto& item : catalog) {
            if (lower(item.mood) == catalogMood && lower(item.type) == preference) items.push_back(&item);
        }
        crow::json::wvalue result;
        result["name"] = name;
        result["mood"] = mood;
        result["preference"] = preference == "movie" ? "Movie" : "Series";
        result["items"] = serialize(items);
        return crow::response(result);
    });

    unsigned short port = 18080;
    if (const char* configuredPort = std::getenv("WHAT2WATCH_PORT")) {
        try {
            const int parsedPort = std::stoi(configuredPort);
            if (parsedPort > 0 && parsedPort <= 65535) port = static_cast<unsigned short>(parsedPort);
        } catch (...) {
            std::cerr << "Invalid WHAT2WATCH_PORT; using 18080.\n";
        }
    }
    app.port(port).multithreaded().run();
}
