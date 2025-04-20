<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:db="http://docbook.org/ns/docbook"
    xmlns:xs="http://www.w3.org/2001/XMLSchema"
    xmlns="http://www.w3.org/1999/xhtml"
    exclude-result-prefixes="db xs"
    version="3.0">
 
<!-- This href has to point to your local copy
     of the stylesheets. -->
<xsl:import href="/usr/share/xml/docbook/stylesheet/nwalsh/fo/docbook.xsl"/>
<!-- Our customizations -->
<!-- Number the section titles. The main reason for this is so that we can refer to
    sections of the specification by number.
    See https://doccookbook.sourceforge.net/html/en/dbc.fo.number-titles.html#sec.fo.number-titles.solution -->
<xsl:param name="section.autolabel" select="1"/>
</xsl:stylesheet>
