# This module is for encoding urls
def url_encode(url):
    url = url.replace("%", "%25")
    url = url.replace(" ", "%20")
    url = url.replace("!", "%21")
    url = url.replace("\"", "%22")
    url = url.replace("#", "%23")
    url = url.replace("$", "%24")
    url = url.replace("&", "%26")
    url = url.replace("'", "%27")
    url = url.replace("(", "%28")
    url = url.replace(")", "%29")
    url = url.replace("*", "%2A")
    url = url.replace("+", "%2B")
    url = url.replace(",", "%2C")
    url = url.replace("-", "%2D")
    url = url.replace(".", "%2E")
    url = url.replace("/", "%2F")
    url = url.replace(":", "%3A")
    url = url.replace(";", "%3B")
    url = url.replace("<", "%3C")
    url = url.replace("=", "%3D")
    url = url.replace(">", "%3E")
    url = url.replace("?", "%3F")
    url = url.replace("@", "%40")

    return url