#include "MediaType.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

void MediaType::add(std::string s, bool isCgi, std::string path, std::string mimeType) {
    mime[s].isCGI = isCgi;
    mime[s].path = path;
    mime[s].mimeType = mimeType;
}

MediaType::MediaType(){
    add("py", true, "/usr/bin/python3", "application/x-python-code");
    add("php", true, "/usr/bin/php-cgi", "application/x-httpd-php");

    add("txt", false, "", "text/plain");
    add("hello", false, "", "hello");
    add("3g2", false, "", "video/3gpp2");
    add("3gp", false, "", "video/3gpp");
    add("3gpp", false, "", "video/3gpp");
    add("ac", false, "", "application/pkix-attr-cert");
    add("adp", false, "", "audio/adpcm");
    add("ai", false, "", "application/postscript");
    add("apng", false, "", "image/apng");
    add("appcache", false, "", "text/cache-manifest");
    add("asc", false, "", "application/pgp-signature");
    add("atom", false, "", "application/atom+xml");
    add("atomcat", false, "", "application/atomcat+xml");
    add("atomsvc", false, "", "application/atomsvc+xml");
    add("au", false, "", "audio/basic");
    add("aw", false, "", "application/applixware");
    add("bdoc", false, "", "application/bdoc");
    add("bin", false, "", "application/octet-stream");
    add("bmp", false, "", "image/bmp");
    add("bpk", false, "", "application/octet-stream");
    add("buffer", false, "", "application/octet-stream");
    add("ccxml", false, "", "application/ccxml+xml");
    add("cdmia", false, "", "application/cdmi-capability");
    add("cdmic", false, "", "application/cdmi-container");
    add("cdmid", false, "", "application/cdmi-domain");
    add("cdmio", false, "", "application/cdmi-object");
    add("cdmiq", false, "", "application/cdmi-queue");
    add("cer", false, "", "application/pkix-cert");
    add("cgm", false, "", "image/cgm");
    add("class", false, "", "application/java-vm");
    add("coffee", false, "", "text/coffeescript");
    add("cpt", false, "", "application/mac-compactpro");
    add("crl", false, "", "application/pkix-crl");
    add("css", false, "", "text/css");
    add("csv", false, "", "text/csv");
    add("cu", false, "", "application/cu-seeme");
    add("davmount", false, "", "application/davmount+xml");
    add("dbk", false, "", "application/docbook+xml");
    add("deb", false, "", "application/octet-stream");
    add("def", false, "", "text/plain");
    add("deploy", false, "", "application/octet-stream");
    add("disposition-notification", false, "", "message/disposition-notification");
    add("dist", false, "", "application/octet-stream");
    add("distz", false, "", "application/octet-stream");
    add("dll", false, "", "application/octet-stream");
    add("dmg", false, "", "application/octet-stream");
    add("dms", false, "", "application/octet-stream");
    add("doc", false, "", "application/msword");
    add("dot", false, "", "application/msword");
    add("drle", false, "", "image/dicom-rle");
    add("dssc", false, "", "application/dssc+der");
    add("dtd", false, "", "application/xml-dtd");
    add("dump", false, "", "application/octet-stream");
    add("ear", false, "", "application/java-archive");
    add("ecma", false, "", "application/ecmascript");
    add("elc", false, "", "application/octet-stream");
    add("emf", false, "", "image/emf");
    add("eml", false, "", "message/rfc822");
    add("emma", false, "", "application/emma+xml");
    add("eps", false, "", "application/postscript");
    add("epub", false, "", "application/epub+zip");
    add("es", false, "", "application/ecmascript");
    add("exe", false, "", "application/octet-stream");
    add("exi", false, "", "application/exi");
    add("exr", false, "", "image/aces");
    add("ez", false, "", "application/andrew-inset");
    add("fits", false, "", "image/fits");
    add("g3", false, "", "image/g3fax");
    add("gbr", false, "", "application/rpki-ghostbusters");
    add("geojson", false, "", "application/geo+json");
    add("gif", false, "", "image/gif");
    add("glb", false, "", "model/gltf-binary");
    add("gltf", false, "", "model/gltf+json");
    add("gml", false, "", "application/gml+xml");
    add("gpx", false, "", "application/gpx+xml");
    add("gram", false, "", "application/srgs");
    add("grxml", false, "", "application/srgs+xml");
    add("gxf", false, "", "application/gxf");
    add("gz", false, "", "application/gzip");
    add("h261", false, "", "video/h261");
    add("h263", false, "", "video/h263");
    add("h264", false, "", "video/h264");
    add("heic", false, "", "image/heic");
    add("heics", false, "", "image/heic-sequence");
    add("heif", false, "", "image/heif");
    add("heifs", false, "", "image/heif-sequence");
    add("hjson", false, "", "application/hjson");
    add("hlp", false, "", "application/winhlp");
    add("hqx", false, "", "application/mac-binhex40");
    add("htm", false, "", "text/html");
    add("html", false, "", "text/html");
    add("ics", false, "", "text/calendar");
    add("ief", false, "", "image/ief");
    add("ifb", false, "", "text/calendar");
    add("iges", false, "", "model/iges");
    add("igs", false, "", "model/iges");
    add("img", false, "", "application/octet-stream");
    add("in", false, "", "text/plain");
    add("ini", false, "", "text/plain");
    add("ink", false, "", "application/inkml+xml");
    add("inkml", false, "", "application/inkml+xml");
    add("ipfix", false, "", "application/ipfix");
    add("iso", false, "", "application/octet-stream");
    add("jade", false, "", "text/jade");
    add("jar", false, "", "application/java-archive");
    add("jls", false, "", "image/jls");
    add("jp2", false, "", "image/jp2");
    add("jpe", false, "", "image/jpeg");
    add("jpeg", false, "", "image/jpeg");
    add("jpf", false, "", "image/jpx");
    add("jpg", false, "", "image/jpeg");
    add("jpg2", false, "", "image/jp2");
    add("jpgm", false, "", "video/jpm");
    add("jpgv", false, "", "video/jpeg");
    add("jpm", false, "", "image/jpm");
    add("jpx", false, "", "image/jpx");
    add("js", false, "", "application/javascript");
    add("json", false, "", "application/json");
    add("json5", false, "", "application/json5");
    add("jsonld", false, "", "application/ld+json");
    add("jsonml", false, "", "application/jsonml+json");
    add("jsx", false, "", "text/jsx");
    add("kar", false, "", "audio/midi");
    add("ktx", false, "", "image/ktx");
    add("less", false, "", "text/less");
    add("list", false, "", "text/plain");
    add("litcoffee", false, "", "text/coffeescript");
    add("log", false, "", "text/plain");
    add("lostxml", false, "", "application/lost+xml");
    add("lrf", false, "", "application/octet-stream");
    add("m1v", false, "", "video/mpeg");
    add("m21", false, "", "application/mp21");
    add("m2a", false, "", "audio/mpeg");
    add("m2v", false, "", "video/mpeg");
    add("m3a", false, "", "audio/mpeg");
    add("m4a", false, "", "audio/mp4");
    add("m4p", false, "", "application/mp4");
    add("ma", false, "", "application/mathematica");
    add("mads", false, "", "application/mads+xml");
    add("man", false, "", "text/troff");
    add("manifest", false, "", "text/cache-manifest");
    add("map", false, "", "application/json");
    add("mar", false, "", "application/octet-stream");
    add("markdown", false, "", "text/markdown");
    add("mathml", false, "", "application/mathml+xml");
    add("mb", false, "", "application/mathematica");
    add("mbox", false, "", "application/mbox");
    add("md", false, "", "text/markdown");
    add("me", false, "", "text/troff");
    add("mesh", false, "", "model/mesh");
    add("meta4", false, "", "application/metalink4+xml");
    add("metalink", false, "", "application/metalink+xml");
    add("mets", false, "", "application/mets+xml");
    add("mft", false, "", "application/rpki-manifest");
    add("mid", false, "", "audio/midi");
    add("midi", false, "", "audio/midi");
    add("mime", false, "", "message/rfc822");
    add("mj2", false, "", "video/mj2");
    add("mjp2", false, "", "video/mj2");
    add("mjs", false, "", "application/javascript");
    add("mml", false, "", "text/mathml");
    add("mods", false, "", "application/mods+xml");
    add("mov", false, "", "video/quicktime");
    add("mp2", false, "", "audio/mpeg");
    add("mp21", false, "", "application/mp21");
    add("mp2a", false, "", "audio/mpeg");
    add("mp3", false, "", "audio/mpeg");
    add("mp4", false, "", "video/mp4");
    add("mp4a", false, "", "audio/mp4");
    add("mp4s", false, "", "application/mp4");
    add("mp4v", false, "", "video/mp4");
    add("mpd", false, "", "application/dash+xml");
    add("mpe", false, "", "video/mpeg");
    add("mpeg", false, "", "video/mpeg");
    add("mpg", false, "", "video/mpeg");
    add("mpg4", false, "", "video/mp4");
    add("mpga", false, "", "audio/mpeg");
    add("mrc", false, "", "application/marc");
    add("mrcx", false, "", "application/marcxml+xml");
    add("ms", false, "", "text/troff");
    add("mscml", false, "", "application/mediaservercontrol+xml");
    add("msh", false, "", "model/mesh");
    add("msi", false, "", "application/octet-stream");
    add("msm", false, "", "application/octet-stream");
    add("msp", false, "", "application/octet-stream");
    add("mxf", false, "", "application/mxf");
    add("mxml", false, "", "application/xv+xml");
    add("n3", false, "", "text/n3");
    add("nb", false, "", "application/mathematica");
    add("oda", false, "", "application/oda");
    add("oga", false, "", "audio/ogg");
    add("ogg", false, "", "audio/ogg");
    add("ogv", false, "", "video/ogg");
    add("ogx", false, "", "application/ogg");
    add("omdoc", false, "", "application/omdoc+xml");
    add("onepkg", false, "", "application/onenote");
    add("onetmp", false, "", "application/onenote");
    add("onetoc", false, "", "application/onenote");
    add("onetoc2", false, "", "application/onenote");
    add("opf", false, "", "application/oebps-package+xml");
    add("otf", false, "", "font/otf");
    add("owl", false, "", "application/rdf+xml");
    add("oxps", false, "", "application/oxps");
    add("p10", false, "", "application/pkcs10");
    add("p7c", false, "", "application/pkcs7-mime");
    add("p7m", false, "", "application/pkcs7-mime");
    add("p7s", false, "", "application/pkcs7-signature");
    add("p8", false, "", "application/pkcs8");
    add("pdf", false, "", "application/pdf");
    add("pfr", false, "", "application/font-tdpfr");
    add("pgp", false, "", "application/pgp-encrypted");
    add("pkg", false, "", "application/octet-stream");
    add("pki", false, "", "application/pkixcmp");
    add("pkipath", false, "", "application/pkix-pkipath");
    add("pls", false, "", "application/pls+xml");
    add("png", false, "", "image/png");
    add("prf", false, "", "application/pics-rules");
    add("ps", false, "", "application/postscript");
    add("pskcxml", false, "", "application/pskc+xml");
    add("qt", false, "", "video/quicktime");
    add("raml", false, "", "application/raml+yaml");
    add("rdf", false, "", "application/rdf+xml");
    add("rif", false, "", "application/reginfo+xml");
    add("rl", false, "", "application/resource-lists+xml");
    add("rld", false, "", "application/resource-lists-diff+xml");
    add("rmi", false, "", "audio/midi");
    add("rnc", false, "", "application/relax-ng-compact-syntax");
    add("rng", false, "", "application/xml");
    add("roa", false, "", "application/rpki-roa");
    add("roff", false, "", "text/troff");
    add("rq", false, "", "application/sparql-query");
    add("rs", false, "", "application/rls-services+xml");
    add("rsd", false, "", "application/rsd+xml");
    add("rss", false, "", "application/rss+xml");
    add("rtf", false, "", "application/rtf");
    add("rtx", false, "", "text/richtext");
    add("s3m", false, "", "audio/s3m");
    add("sbml", false, "", "application/sbml+xml");
    add("scq", false, "", "application/scvp-cv-request");
    add("scs", false, "", "application/scvp-cv-response");
    add("sdp", false, "", "application/sdp");
    add("ser", false, "", "application/java-serialized-object");
    add("setpay", false, "", "application/set-payment-initiation");
    add("setreg", false, "", "application/set-registration-initiation");
    add("sgi", false, "", "image/sgi");
    add("sgm", false, "", "text/sgml");
    add("sgml", false, "", "text/sgml");
    add("shex", false, "", "text/shex");
    add("shf", false, "", "application/shf+xml");
    add("shtml", false, "", "text/html");
    add("sig", false, "", "application/pgp-signature");
    add("sil", false, "", "audio/silk");
    add("silo", false, "", "model/mesh");
    add("slim", false, "", "text/slim");
    add("slm", false, "", "text/slim");
    add("smi", false, "", "application/smil+xml");
    add("smil", false, "", "application/smil+xml");
    add("snd", false, "", "audio/basic");
    add("so", false, "", "application/octet-stream");
    add("spp", false, "", "application/scvp-vp-response");
    add("spq", false, "", "application/scvp-vp-request");
    add("spx", false, "", "audio/ogg");
    add("sru", false, "", "application/sru+xml");
    add("srx", false, "", "application/sparql-results+xml");
    add("ssdl", false, "", "application/ssdl+xml");
    add("ssml", false, "", "application/ssml+xml");
    add("stk", false, "", "application/hyperstudio");
    add("styl", false, "", "text/stylus");
    add("stylus", false, "", "text/stylus");
    add("svg", false, "", "image/svg+xml");
    add("svgz", false, "", "image/svg+xml");
    add("t", false, "", "text/troff");
    add("t38", false, "", "image/t38");
    add("tei", false, "", "application/tei+xml");
    add("teicorpus", false, "", "application/tei+xml");
    add("text", false, "", "text/plain");
    add("tfi", false, "", "application/thraud+xml");
    add("tfx", false, "", "image/tiff-fx");
    add("tif", false, "", "image/tiff");
    add("tiff", false, "", "image/tiff");
    add("tr", false, "", "text/troff");
    add("ts", false, "", "video/mp2t");
    add("tsd", false, "", "application/timestamped-data");
    add("tsv", false, "", "text/tab-separated-values");
    add("ttc", false, "", "font/collection");
    add("ttf", false, "", "font/ttf");
    add("ttl", false, "", "text/turtle");
    add("u8dsn", false, "", "message/global-delivery-status");
    add("u8hdr", false, "", "message/global-headers");
    add("u8mdn", false, "", "message/global-disposition-notification");
    add("u8msg", false, "", "message/global");
    add("uri", false, "", "text/uri-list");
    add("uris", false, "", "text/uri-list");
    add("urls", false, "", "text/uri-list");
    add("vcard", false, "", "text/vcard");
    add("vrml", false, "", "model/vrml");
    add("vtt", false, "", "text/vtt");
    add("vxml", false, "", "application/voicexml+xml");
    add("war", false, "", "application/java-archive");
    add("wasm", false, "", "application/wasm");
    add("wav", false, "", "audio/wav");
    add("weba", false, "", "audio/webm");
    add("webm", false, "", "video/webm");
    add("webmanifest", false, "", "application/manifest+json");
    add("webp", false, "", "image/webp");
    add("wgt", false, "", "application/widget");
    add("wmf", false, "", "image/wmf");
    add("woff", false, "", "font/woff");
    add("woff2", false, "", "font/woff2");
    add("wrl", false, "", "model/vrml");
    add("wsdl", false, "", "application/wsdl+xml");
    add("wspolicy", false, "", "application/wspolicy+xml");
    add("x3d", false, "", "model/x3d+xml");
    add("x3db", false, "", "model/x3d+binary");
    add("x3dbz", false, "", "model/x3d+binary");
    add("x3dv", false, "", "model/x3d+vrml");
    add("x3dvz", false, "", "model/x3d+vrml");
    add("x3dz", false, "", "model/x3d+xml");
    add("xaml", false, "", "application/xaml+xml");
    add("xdf", false, "", "application/xcap-diff+xml");
    add("xdssc", false, "", "application/dssc+xml");
    add("xenc", false, "", "application/xenc+xml");
    add("xer", false, "", "application/patch-ops-error+xml");
    add("xht", false, "", "application/xhtml+xml");
    add("xhtml", false, "", "application/xhtml+xml");
    add("xhvml", false, "", "application/xv+xml");
    add("xm", false, "", "audio/xm");
    add("xml", false, "", "application/xml");
    add("xop", false, "", "application/xop+xml");
    add("xpl", false, "", "application/xproc+xml");
    add("xsd", false, "", "application/xml");
    add("xsl", false, "", "application/xml");
    add("xslt", false, "", "application/xslt+xml");
    add("xspf", false, "", "application/xspf+xml");
    add("xvm", false, "", "application/xv+xml");
    add("xvml", false, "", "application/xv+xml");
    add("yaml", false, "", "text/yaml");
    add("yang", false, "", "application/yang");
    add("yin", false, "", "application/yin+xml");
    add("yml", false, "", "text/yaml");
    add("zip", false, "", "application/zip");
    add("ico", false, "", "image/x-icon");
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

MediaType::~MediaType(){
}

std::string MediaType::getMediaType(std::string requestedExtension) {
    // std::cout << std::endl << RED "Request : " << requestedExtension << WHITE << std::endl;
    requestedExtension.erase(std::remove_if(requestedExtension.begin(), requestedExtension.end(), ::isspace),
            requestedExtension.end()); // remove all space
    // std::cout << std::endl << PURPLE "Request : " << requestedExtension << PURPLE << WHITE << std::endl;
    while (requestedExtension.find('/') != std::string::npos) // reach the file
    {
        requestedExtension = requestedExtension.substr(requestedExtension.find('/') + 1);
    } 
    // std::cout << CYAN << "final : " << requestedExtension << WHITE << std::endl;
    while (requestedExtension.find('.') != std::string::npos) // reach the extension
    {
        requestedExtension = requestedExtension.substr(requestedExtension.find('.') + 1);
    } 
    // std::cout << GREEN << "final : " << requestedExtension << WHITE << std::endl;
    return (mime.at(requestedExtension).mimeType); // searching extension in mime // at throw an exception if it's not find
}

std::string MediaType::getMediaExtension(std::string requestedType)
{
    if (requestedType.find('/') == std::string::npos)
        return (NULL);
    requestedType.erase(std::remove_if(requestedType.begin(), requestedType.end(), ::isspace),
        requestedType.end()); // remove all space
    for(std::map<std::string, t_data>::iterator it = mime.begin(); it != mime.end(); ++it)
    {
        if(mime[(*it).first].mimeType == requestedType)
            return ((*it).first);
    }
    return (NULL);
}

// peut contenir une wildcart
// requestedTypes : text/*;q=0.3, text/plain;q=0.7, text/plain;format=flowed, text/plain;format=fixed;q=0.4, */*;q=0.5
// fileFormat : Mon file au format type
bool 		MediaType::isInAccepted(std::string accepted, std::string fileFormat) //(recoit la ligne complete de "Accepted: " et la parse (du coup fait les ordre de prio et tout)) et check si ce quon sapprete a renvoyer et dans accepted //TODO
{
    accepted.erase(std::remove_if(accepted.begin(), accepted.end(), ::isspace),
        accepted.end()); // remove all space
    
    while (1){
        std::string parsed = accepted.substr(0, accepted.find(','));
        if (parsed.find(';') != std::string::npos)
            parsed = parsed.substr(0, accepted.find(';'));
        // std::cout << "Element : " << parsed << "\tfind : " << isTypeSupported(parsed) << std::endl;

        //wildcard
        {
            if (parsed.find('*') != std::string::npos)
            {
                std::string first = parsed.substr(0, parsed.find('/'));
                std::string second = parsed.substr(parsed.find('/') + 1);
                if (first.find('*') != std::string::npos && second.find('*') != std::string::npos)
                    return (true);
                if (second.find('*') != std::string::npos)
                {
                    for(std::map<std::string, t_data>::iterator it = mime.begin(); it != mime.end(); ++it)
                    {
                        std::string full_extension = first + '/' + mime[(*it).first].mimeType.substr(mime[(*it).first].mimeType.find('/') + 1);
                        if (isTypeSupported(full_extension)) {
                            // std::cout << RED << fileFormat << PURPLE "Full : " << full_extension << WHITE << std::endl;
                            if (getMediaExtension(full_extension) == fileFormat)
                                return (true);
                        }
                    }
                }
            }
        }

        // if (isTypeSupported(parsed))
        //     std::cout << "\t\t" << getMediaExtension(parsed) << std::endl;
        if (isTypeSupported(parsed))
            if (getMediaExtension(parsed) == fileFormat)
                return (true);
        // std::cout << accepted << "\n"; 
        if (accepted.find(',') == std::string::npos)
            break;
        accepted = accepted.substr(accepted.find(',') + 1);
        // std::cout << "Accepted : " << accepted << std::endl;
    }
    return (false);
}

bool	MediaType::isTypeSupported(std::string	requestedType) //recoit la ligne dans contentType et check si le type est bien un media type accepté //TODO
{
    // std::cout << std::endl << RED "Giving : " << requestedType << RED << WHITE << std::endl;
    requestedType.erase(std::remove_if(requestedType.begin(), requestedType.end(), ::isspace),
        requestedType.end()); // remove all space
    // std::cout << std::endl << PURPLE "Space : " << requestedType << PURPLE << WHITE << std::endl;
    requestedType = requestedType.substr(0, requestedType.find(';'));
    if (requestedType.find('/') == std::string::npos)
        return (false); //comporte un /
    // std::cout << std::endl << CYAN "After ';' : '" << requestedType << "'" CYAN << WHITE << std::endl;
    for(std::map<std::string, t_data>::iterator it = mime.begin(); it != mime.end(); ++it)
    {
        if(mime[(*it).first].mimeType == requestedType)
            return (true);
    }
    return (false);
}

bool	MediaType::isExtensionSupported(std::string requestedExtension)
{
    requestedExtension.erase(std::remove_if(requestedExtension.begin(), requestedExtension.end(), ::isspace),
        requestedExtension.end()); // remove all space
    requestedExtension = requestedExtension.substr(requestedExtension.find_last_of('.') + 1); //reach the extension
    if (mime.find(requestedExtension) == mime.end())
        return (false);
    return (true);
}


std::string MediaType::getProgName(std::string &extension) {
    std::map<std::string, t_data>::iterator it;
    it = mime.find(extension);
    if (it != mime.end())
        return (it->second.mimeType);
    exit(123); //throw
}

/* ************************************************************************** */