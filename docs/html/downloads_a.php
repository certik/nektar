<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml"><!-- InstanceBegin template="/Templates/Main.dwt" codeOutsideHTMLIsLocked="false" -->
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link rel="shortcut icon" href="images/favicon.ico" >
    <!-- InstanceBeginEditable name="doctitle" -->
    <title>Nektar++ - Downloads Analysis</title>
    <!-- InstanceEndEditable -->
    <style type="text/css">
    <!--
	html {
	  min-height: 101%; // force a scrollbar to always appear
	}
    body {
      font: 100% Verdana, Arial, Helvetica, sans-serif;
      background: #FFFFFF;
      margin: 0;
	  padding: 0;
	  text-align: center;
	  color: #000000;
    }
    .twoColHybLt #container {
	  //max-width: 90em;
	  max-width: 60em;
	  width: 99%;
      background: #FFFFFF;
      margin: 0 auto;
      text-align: left;
    }
    .twoColHybLt #sidebar1 {
	float: left;
	width: 13.7em;
	background: #EBEBEB;
	border: 1px solid #000000;
	padding-top: 15px;
	padding-right: 0;
	padding-bottom: 15px;
	padding-left: 0;
    }
    .twoColHybLt #sidebar1 h3, .twoColHybLt #sidebar1 p {
      margin-left: 10px;
      margin-right: 10px;
    }
    .twoColHybLt #mainContent {
	margin-top: 0;
	margin-right: 20px;
	margin-bottom: 0;
	margin-left: 14.5em;
    }
    .fltrt {
      float: right;
      margin-left: 8px;
    }
    .fltlft {
      float: left;
      margin-right: 8px;
    }
    .clearfloat {
      clear:both;
      height:0;
      font-size: 1px;
      line-height: 0px;
    }
    img {
      border:0;
    }
	h1,h2,h3 {
	  margin-bottom: 0;
	}
    -->
    </style>
    <!-- InstanceBeginEditable name="CSS_Style" -->
    <style type="text/css">
    <!--
    #menu_Nektar {
      font-weight: bold;
    }
    -->
    </style>
    <!-- InstanceEndEditable -->
    <!--[if IE]>
    <style type="text/css"> 
      .twoColHybLt #sidebar1 { padding-top: 30px; margin-top: 20px; }
      .twoColHybLt #mainContent { zoom: 1; padding-top: 15px; }
    </style>
    <![endif]-->
    <!-- InstanceParam name="LibraryMenu" type="boolean" value="false" -->
    <!-- InstanceParam name="CompileMenu" type="boolean" value="false" -->
  </head>
  <body class="twoColHybLt">
    <div id="container">
      <div id="sidebar1">
        <h3 style="text-align: center;"><a href="index.html" id="menu_Nektar"><img src="images/nektar.png" alt="Nektar++" width="155" height="38" /></a></h3>
        <p><a href="downloads.html" id="menu_Downloads">Downloads</a></p>
        <p>
          <a href="compile.html" id="menu_Compile">Compile Instructions</a>
    	  
        </p>
        <p>
          <a href="library.html" id="menu_Library">Library Overview</a>
    	  
        </p>
        <p><a href="team.html" id="menu_Team">Team Members</a></p>
<!--        <p><a href="pre-processing.html" id="menu_PreProcessing">Pre-processing</a></p>-->
<!--        <p><a href="solvers.html" id="menu_Solvers">Solvers</a></p>-->
        <p><a href="post-processing.html" id="menu_PostProcessing">Post-processing</a></p>
<!--        <p><a href="demos.html" id="menu_Demos">Demos</a></p>-->
        <p><a href="http://www.nektar.info/2nd_edition/" target="_blank" id="menu_Book2ndEd">Spectral/hp element methods for CFD</a> (Karniadakis and Sherwin)</p>
        <p><a href="educational_material.html" id="menu_EducationalMaterial">Educational Material</a></p>
        <p><a href="license.html" id="menu_License">License</a></p>
        <p><a href="acknowledgments.html" id="menu_Acknowledgments">Acknowledgments</a></p>
        <p><a href="mailto:nektar-inquiry@sci.utah.edu" id="menu_Contact">Contact</a></p>
        <p style="font-size:9px;">Last Revision:<br/>$Date: 2008/06/05 21:31:53 $</p>
      </div>
      <div id="mainContent">
        <!-- InstanceBeginEditable name="Title" -->
        <h1>Downloads Analysis</h1>
    	<!-- InstanceEndEditable -->
        <hr/>
        <!-- InstanceBeginEditable name="BodyText" -->
<?php
// Load connection constants.
require('common.php');

// Connect to MySQL.
$link = mysql_connect($hostname, $username, $password);
if (!$link)
	die("Cannot connect to MySQL.");

// Select the correct database.
$db_selected = mysql_select_db($database, $link);
if (!$db_selected)
	die("Cannot use the specified database.");
?>
<?php
if (!isset($_GET["mode"]) || $_GET["mode"] == "general")
{
?>
        <h3>Overall Downloads</h3>
        <table border="1">
          <tr>
            <td style="text-align: center; padding-left: 10px; padding-right: 10px;">Affiliation</td>
            <td style="text-align: center; padding-left: 10px; padding-right: 10px;">Download Count</td>
            <td style="text-align: center; padding-left: 10px; padding-right: 10px;">Last Download</td>
          </tr>
<?php
$result = mysql_query("SELECT `Affiliation`, COUNT(*) as `Count`, MAX(`Date`) as `LastDownload` FROM `$table` GROUP BY `Affiliation` ORDER BY `Count` DESC, `Affiliation`", $link);
while ($row = mysql_fetch_assoc($result))
{
	echo "          <tr>";
    echo "            <td style=\"padding-left: 10px; padding-right: 10px;\">" . $row['Affiliation'] . "</td>";
    echo "            <td style=\"padding-left: 10px; padding-right: 10px;\">" . $row['Count'] . "</td>";
    echo "            <td style=\"padding-left: 10px; padding-right: 10px;\">" . $row['LastDownload'] . "</td>";
	echo "          </tr>";
}
?>
        </table>
        <p>&nbsp;</p>
        <h3>Last 30 Days</h3>
        <table border="1">
          <tr>
            <td style="text-align: center; padding-left: 10px; padding-right: 10px;">Affiliation</td>
            <td style="text-align: center; padding-left: 10px; padding-right: 10px;">Download Count</td>
            <td style="text-align: center; padding-left: 10px; padding-right: 10px;">Last Download</td>
          </tr>
<?php
$result = mysql_query("SELECT `Affiliation`, COUNT(*) as `Count`, MAX(`Date`) as `LastDownload` FROM `$table` WHERE `Date` BETWEEN (NOW() - INTERVAL 30 DAY) AND NOW() GROUP BY `Affiliation` ORDER BY `Count` DESC, `Affiliation`", $link);
while ($row = mysql_fetch_assoc($result))
{
	echo "          <tr>";
    echo "            <td style=\"padding-left: 10px; padding-right: 10px;\">" . $row['Affiliation'] . "</td>";
    echo "            <td style=\"padding-left: 10px; padding-right: 10px;\">" . $row['Count'] . "</td>";
    echo "            <td style=\"padding-left: 10px; padding-right: 10px;\">" . $row['LastDownload'] . "</td>";
	echo "          </tr>";
}
?>
        </table>
        <p>&nbsp;</p>
		<p><a href="?mode=people">Switch to Listings of People</a></p>
        <p>&nbsp;</p>
<?php
}
else if ($_GET["mode"] == "people")
{
?>
		<h3>Individual Downloads</h3>
        <table border="1">
          <tr>
            <td style="text-align: center; padding-left: 10px; padding-right: 10px;">ID</td>
            <td style="text-align: center; padding-left: 10px; padding-right: 10px;">Name</td>
            <td style="text-align: center; padding-left: 10px; padding-right: 10px;">E-mail</td>
            <td style="text-align: center; padding-left: 10px; padding-right: 10px;">Affiliation</td>
            <td style="text-align: center; padding-left: 10px; padding-right: 10px;">Date</td>
          </tr>
<?php
$result = mysql_query("SELECT `ID`, `Name`, `Email`, `Affiliation`, `Date` FROM `$table` ORDER BY `Date` DESC", $link);
while ($row = mysql_fetch_assoc($result))
{
	echo "          <tr>";
    echo "            <td style=\"padding-left: 10px; padding-right: 10px;\">" . htmlentities($row['ID']) . "</td>";
    echo "            <td style=\"padding-left: 10px; padding-right: 10px;\">" . htmlentities($row['Name']) . "</td>";
    echo "            <td style=\"padding-left: 10px; padding-right: 10px;\"><a href=\"mailto:". htmlentities($row['Email']) . "\">" . htmlentities($row['Email']) . "</a></td>";
    echo "            <td style=\"padding-left: 10px; padding-right: 10px;\">" . htmlentities($row['Affiliation']) . "</td>";
    echo "            <td style=\"padding-left: 10px; padding-right: 10px;\">" . htmlentities($row['Date']) . "</td>";
	echo "          </tr>";
}
?>
        </table>
        <p>&nbsp;</p>
		<p><a href="?mode=general">Switch to Stats Page</a></p>
        <p>&nbsp;</p>
<?php
}
?>
<?php
// Close the connection to MySQL.
mysql_close($link);
?>
        <!-- InstanceEndEditable -->
        <p><br class="clearfloat" /></p>
      </div>
    </div>
  </body>
<!-- InstanceEnd --></html>