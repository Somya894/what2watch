const API_BASE = "";
const FALLBACK_POSTER = "/poster-fallback.svg";
let selectedMood = "";

function selectMood(mood, button) {
    selectedMood = mood;
    document.querySelectorAll(".mood-buttons button").forEach((option) => {
        const isSelected = option === button;
        option.classList.toggle("selected", isSelected);
        option.setAttribute("aria-pressed", String(isSelected));
    });
    document.getElementById("moodError").hidden = true;
}

function renderMedia(container, items) {
    container.replaceChildren();
    items.forEach((item) => {
        const card = document.createElement("article");
        card.className = "movie-card";

        const poster = document.createElement("div");
        poster.className = "poster";
        const image = document.createElement("img");
        const posterUrl = typeof item.poster === "string" ? item.poster.trim() : "";
        image.alt = posterUrl ? `${item.title} poster` : `Poster unavailable for ${item.title}`;
        // The catalog is small enough to request posters eagerly, so every card
        // can settle to a real image or its fallback without waiting for scroll.
        image.loading = "eager";
        let fallbackTimer = null;
        const showFallback = () => {
            if (image.dataset.fallbackApplied === "true") return;
            image.dataset.fallbackApplied = "true";
            image.alt = `Poster unavailable for ${item.title}`;
            image.src = FALLBACK_POSTER;
        };
        image.onerror = () => {
            if (fallbackTimer !== null) window.clearTimeout(fallbackTimer);
            showFallback();
        };
        image.onload = () => {
            if (fallbackTimer !== null) window.clearTimeout(fallbackTimer);
            fallbackTimer = null;
        };
        if (posterUrl) {
            image.src = posterUrl;
            fallbackTimer = window.setTimeout(() => {
                if (!image.complete || image.naturalWidth === 0) showFallback();
            }, 12000);
        } else {
            image.dataset.fallbackApplied = "true";
            image.src = FALLBACK_POSTER;
        }
        poster.appendChild(image);

        const title = document.createElement("h3");
        title.textContent = item.title;
        const details = document.createElement("p");
        const duration = item.type === "Movie"
            ? `${item.durationMinutes} min`
            : `${item.seasons} ${item.seasons === 1 ? "season" : "seasons"}`;
        details.textContent = `${item.genre} · ${Number(item.rating).toFixed(1)}/10 · ${duration}`;
        const metadata = document.createElement("p");
        metadata.className = "movie-metadata";
        metadata.textContent = [item.year, item.language].filter(Boolean).join(" · ");
        card.append(poster, title, details);
        if (metadata.textContent) card.append(metadata);
        container.appendChild(card);
    });
}

async function loadCatalog() {
    const status = document.getElementById("catalogStatus");
    status.textContent = "Loading the catalog…";
    status.hidden = false;
    try {
        const response = await fetch(`${API_BASE}/api/media`);
        if (!response.ok) throw new Error("Could not load the catalog.");
        const items = await response.json();
        renderMedia(document.getElementById("movieContainer"), items.filter((item) => item.type === "Movie"));
        renderMedia(document.getElementById("seriesContainer"), items.filter((item) => item.type === "Series"));
        status.hidden = true;
    } catch (error) {
        status.textContent = "Could not load the catalog. What2Watch is unavailable; start it and refresh this page.";
        status.hidden = false;
    }
}

async function searchMovies() {
    const input = document.getElementById("searchInput");
    const query = input.value.trim();
    const noResults = document.getElementById("noResults");
    const searchStatus = document.getElementById("searchStatus");
    searchStatus.textContent = "Searching…";
    searchStatus.hidden = false;
    noResults.hidden = true;
    try {
        const response = await fetch(`${API_BASE}/api/search?q=${encodeURIComponent(query)}`);
        if (!response.ok) throw new Error("Search failed.");
        const items = await response.json();
        searchStatus.hidden = true;
        renderMedia(document.getElementById("movieContainer"), items.filter((item) => item.type === "Movie"));
        renderMedia(document.getElementById("seriesContainer"), items.filter((item) => item.type === "Series"));
        noResults.hidden = items.length !== 0;
        document.getElementById("movies").scrollIntoView({ behavior: "smooth", block: "start" });
    } catch (error) {
        searchStatus.textContent = "Search is unavailable because What2Watch could not be reached.";
        searchStatus.hidden = false;
    }
}

async function getRecommendations() {
    const nameInput = document.getElementById("userName");
    const name = nameInput.value.trim();
    const nameError = document.getElementById("nameError");
    const moodError = document.getElementById("moodError");
    const welcomeMessage = document.getElementById("welcomeMessage");
    const results = document.getElementById("recommendationResults");
    const status = document.getElementById("recommendationStatus");

    if (!name) {
        nameError.hidden = false;
        moodError.hidden = true;
        welcomeMessage.textContent = "";
        nameInput.focus();
        return;
    }

    nameError.hidden = true;
    if (!selectedMood) {
        moodError.hidden = false;
        document.querySelector(".mood-buttons button")?.focus();
        return;
    }
    moodError.hidden = true;
    results.replaceChildren();
    status.textContent = "Finding recommendations…";
    status.hidden = false;
    try {
        const preference = document.getElementById("mediaPreference").value;
        const params = new URLSearchParams({ name, mood: selectedMood, preference });
        const response = await fetch(`${API_BASE}/api/recommendations?${params}`);
        const payload = await response.json();
        if (!response.ok) throw new Error(payload.error || "Could not load recommendations.");
        welcomeMessage.textContent = `Welcome, ${payload.name}! Your selected mood: ${payload.mood}`;
        status.hidden = true;
        if (payload.items.length === 0) {
            status.textContent = `No ${preference === "Movie" ? "movies" : "series"} found for this mood yet.`;
            status.hidden = false;
            return;
        }
        const heading = document.createElement("h2");
        heading.textContent = `${payload.name}, here are your ${preference === "Movie" ? "movie" : "series"} recommendations!`;
        renderMedia(results, payload.items);
        results.prepend(heading);
        results.scrollIntoView({ behavior: "smooth", block: "start" });
    } catch (error) {
        status.textContent = error instanceof TypeError
            ? "Could not reach What2Watch. Start the app and try again."
            : error.message || "Could not load recommendations. Try again.";
        status.hidden = false;
    }
}

document.getElementById("searchInput").addEventListener("keydown", (event) => {
    if (event.key === "Enter") searchMovies();
});

loadCatalog();
