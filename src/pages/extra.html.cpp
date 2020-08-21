#include <string>

extern const std::string extra_html=R"html(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>OpenWinch - Extra</title>
    <meta name="description" content="Winch controller."/>
    <meta name="author" content="Mickael Gaillard" />
    <meta name="copyright" content="Mickael Gaillard" />
    <meta name="robots" content="noindex"/>
    <meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="mobile-web-app-capable" content="yes">
    <meta http-equiv="cache-control" content="no-cache"/>
    <meta http-equiv="expires" content="0"/>
    <link rel="stylesheet" href="/css/style.css">
    <style type="text/css">
        .important { color: #336699; }
    </style>
  </head>
  <body>
    <div id="content">
      <div id="header">
        <noscript>To use OpenWinch, please enable JavaScript<br></noscript>
        <h1>OpenWinch</h1>
      </div>
      <div id="cmd">
        <a href="/reset"><button class="btn-cmd" style="background-color: rgb(230, 167, 31);">Reset</button></a>
        <table style="width: 100%;">
          <tbody >
            <tr><th><a href="/left"><button class="btn-cmd" style="background-color: green;font-size: 6.2rem;"><<<</button></a>
            </th><th><a href="/right"><button class="btn-cmd" style="background-color: crimson;font-size: 6.2rem;">>>></button></a>
            </th></tr>
          </tbody>
        </table>
        <a href="/"><button class="btn-speed">Back</button></a>
      </div>
      <footer>
        <hr>
        <a href="https://bit.ly/openwinch" target="_blank" style="color:#aaa;">OpenWinch {{ app_version }} by Mickael Gaillard</a>
      </footer>
    </div>
  </body>
</html>
)html";
