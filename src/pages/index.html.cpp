#include <string>

extern const std::string index_html=R"html(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>OpenWinch - Control</title>
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
    <script language="javascript">
      setInterval(function(){
        url = '{{ request.url_root }}';
        window.location.assign(url);
      }, 3000);
    </script>
  </head>
  <body>
    <div id="content">
      <div id="header">
        <noscript>To use OpenWinch, please enable JavaScript<br></noscript>
        <h1>OpenWinch</h1>
      </div>
      <div id="info">
        <table>
          <tbody>
            <tr><th style="width: 230px;;">Mode</th><td>{{ mode }}</td></tr>
            <tr><th>Bat</th><td>{{ battery }} %</td></tr>
          </tbody>
        </table>
      </div>
      <div id="speed">
        <a href="/up"><button class="btn-speed">+</button></a>
        <div id="value-speed" style="color: {{ enable }};">{{ speed_target }} {{ speed_unit }}</div>
        <a href="/down"><button class="btn-speed">-</button></a>
      </div>
      <div id="cmd">
        <table style="width: 100%;">
          <tbody >
            <tr><th><a href="/start"><button class="btn-cmd" style="background-color: green;">Start</button></a>
            </th><th><a href="/stop"><button class="btn-cmd" style="background-color: crimson;">Stop</button></a>
            </th></tr>
          </tbody>
        </table>
        <a href="/halt"><button class="btn-cmd" style="background-color: red;">!! EMERGENCY !!</button></a>
        <a href="/extra"><button class="btn-speed">Extra</button></a>
      </div>
      <footer>
        <hr>
        <a href="https://bit.ly/openwinch" target="_blank" style="color:#aaa;">OpenWinch {{ app_version }} by Mickael Gaillard</a>
      </footer>
    </div>
  </body>
</html>
)html";
