To use this code properly. make sure to set up the SFML library properly in your IDE. Make sure all file locations match the path specific to your computer as well.

## Netlify deployment
A lightweight browser build is available in `web/` so the game can be deployed as a static site on Netlify.

1. Push this repository to GitHub.
2. In Netlify, create a new site from Git and set the build command to empty.
3. Set the publish directory to `web` (matching `netlify.toml`).
4. Deploy and play Minesweeper directly in the browser.
