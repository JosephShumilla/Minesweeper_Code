To use this code properly. make sure to set up the SFML library properly in your IDE. Make sure all file locations match the path specific to your computer as well.

## Web deployment

A browser-friendly version now lives at the repository root (`index.html` + `web-app.js`). To preview locally, serve the folder with any static file server (for example, `python -m http.server 8080`) and open the reported URL. Netlify is configured via `netlify.toml` to publish the repository root, reusing the existing image assets and `config.cfg` board layout.
