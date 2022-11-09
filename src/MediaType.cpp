#include "MediaType.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

MediaType::MediaType(){
    mime["py"] = "cgi/python"; //a enlever
    mime["php"] = "cgi/php"; //a enlever
    mime["txt"] = "text/plain";
    mime["hello"] = "hello";
    mime["3g2"] = "video/3gpp2";
    mime["3gp"] = "video/3gpp";
    mime["3gpp"] = "video/3gpp";
    mime["ac"] = "application/pkix-attr-cert";
    mime["adp"] = "audio/adpcm";
    mime["ai"] = "application/postscript";
    mime["apng"] = "image/apng";
    mime["appcache"] = "text/cache-manifest";
    mime["asc"] = "application/pgp-signature";
    mime["atom"] = "application/atom+xml";
    mime["atomcat"] = "application/atomcat+xml";
    mime["atomsvc"] = "application/atomsvc+xml";
    mime["au"] = "audio/basic";
    mime["aw"] = "application/applixware";
    mime["bdoc"] = "application/bdoc";
    mime["bin"] = "application/octet-stream";
    mime["bmp"] = "image/bmp";
    mime["bpk"] = "application/octet-stream";
    mime["buffer"] = "application/octet-stream";
    mime["ccxml"] = "application/ccxml+xml";
    mime["cdmia"] = "application/cdmi-capability";
    mime["cdmic"] = "application/cdmi-container";
    mime["cdmid"] = "application/cdmi-domain";
    mime["cdmio"] = "application/cdmi-object";
    mime["cdmiq"] = "application/cdmi-queue";
    mime["cer"] = "application/pkix-cert";
    mime["cgm"] = "image/cgm";
    mime["class"] = "application/java-vm";
    mime["coffee"] = "text/coffeescript";
    mime["cpt"] = "application/mac-compactpro";
    mime["crl"] = "application/pkix-crl";
    mime["css"] = "text/css";
    mime["csv"] = "text/csv";
    mime["cu"] = "application/cu-seeme";
    mime["davmount"] = "application/davmount+xml";
    mime["dbk"] = "application/docbook+xml";
    mime["deb"] = "application/octet-stream";
    mime["def"] = "text/plain";
    mime["deploy"] = "application/octet-stream";
    mime["disposition-notification"] = "message/disposition-notification";
    mime["dist"] = "application/octet-stream";
    mime["distz"] = "application/octet-stream";
    mime["dll"] = "application/octet-stream";
    mime["dmg"] = "application/octet-stream";
    mime["dms"] = "application/octet-stream";
    mime["doc"] = "application/msword";
    mime["dot"] = "application/msword";
    mime["drle"] = "image/dicom-rle";
    mime["dssc"] = "application/dssc+der";
    mime["dtd"] = "application/xml-dtd";
    mime["dump"] = "application/octet-stream";
    mime["ear"] = "application/java-archive";
    mime["ecma"] = "application/ecmascript";
    mime["elc"] = "application/octet-stream";
    mime["emf"] = "image/emf";
    mime["eml"] = "message/rfc822";
    mime["emma"] = "application/emma+xml";
    mime["eps"] = "application/postscript";
    mime["epub"] = "application/epub+zip";
    mime["es"] = "application/ecmascript";
    mime["exe"] = "application/octet-stream";
    mime["exi"] = "application/exi";
    mime["exr"] = "image/aces";
    mime["ez"] = "application/andrew-inset";
    mime["fits"] = "image/fits";
    mime["g3"] = "image/g3fax";
    mime["gbr"] = "application/rpki-ghostbusters";
    mime["geojson"] = "application/geo+json";
    mime["gif"] = "image/gif";
    mime["glb"] = "model/gltf-binary";
    mime["gltf"] = "model/gltf+json";
    mime["gml"] = "application/gml+xml";
    mime["gpx"] = "application/gpx+xml";
    mime["gram"] = "application/srgs";
    mime["grxml"] = "application/srgs+xml";
    mime["gxf"] = "application/gxf";
    mime["gz"] = "application/gzip";
    mime["h261"] = "video/h261";
    mime["h263"] = "video/h263";
    mime["h264"] = "video/h264";
    mime["heic"] = "image/heic";
    mime["heics"] = "image/heic-sequence";
    mime["heif"] = "image/heif";
    mime["heifs"] = "image/heif-sequence";
    mime["hjson"] = "application/hjson";
    mime["hlp"] = "application/winhlp";
    mime["hqx"] = "application/mac-binhex40";
    mime["htm"] = "text/html";
    mime["html"] = "text/html";
    mime["ics"] = "text/calendar";
    mime["ief"] = "image/ief";
    mime["ifb"] = "text/calendar";
    mime["iges"] = "model/iges";
    mime["igs"] = "model/iges";
    mime["img"] = "application/octet-stream";
    mime["in"] = "text/plain";
    mime["ini"] = "text/plain";
    mime["ink"] = "application/inkml+xml";
    mime["inkml"] = "application/inkml+xml";
    mime["ipfix"] = "application/ipfix";
    mime["iso"] = "application/octet-stream";
    mime["jade"] = "text/jade";
    mime["jar"] = "application/java-archive";
    mime["jls"] = "image/jls";
    mime["jp2"] = "image/jp2";
    mime["jpe"] = "image/jpeg";
    mime["jpeg"] = "image/jpeg";
    mime["jpf"] = "image/jpx";
    mime["jpg"] = "image/jpeg";
    mime["jpg2"] = "image/jp2";
    mime["jpgm"] = "video/jpm";
    mime["jpgv"] = "video/jpeg";
    mime["jpm"] = "image/jpm";
    mime["jpx"] = "image/jpx";
    mime["js"] = "application/javascript";
    mime["json"] = "application/json";
    mime["json5"] = "application/json5";
    mime["jsonld"] = "application/ld+json";
    mime["jsonml"] = "application/jsonml+json";
    mime["jsx"] = "text/jsx";
    mime["kar"] = "audio/midi";
    mime["ktx"] = "image/ktx";
    mime["less"] = "text/less";
    mime["list"] = "text/plain";
    mime["litcoffee"] = "text/coffeescript";
    mime["log"] = "text/plain";
    mime["lostxml"] = "application/lost+xml";
    mime["lrf"] = "application/octet-stream";
    mime["m1v"] = "video/mpeg";
    mime["m21"] = "application/mp21";
    mime["m2a"] = "audio/mpeg";
    mime["m2v"] = "video/mpeg";
    mime["m3a"] = "audio/mpeg";
    mime["m4a"] = "audio/mp4";
    mime["m4p"] = "application/mp4";
    mime["ma"] = "application/mathematica";
    mime["mads"] = "application/mads+xml";
    mime["man"] = "text/troff";
    mime["manifest"] = "text/cache-manifest";
    mime["map"] = "application/json";
    mime["mar"] = "application/octet-stream";
    mime["markdown"] = "text/markdown";
    mime["mathml"] = "application/mathml+xml";
    mime["mb"] = "application/mathematica";
    mime["mbox"] = "application/mbox";
    mime["md"] = "text/markdown";
    mime["me"] = "text/troff";
    mime["mesh"] = "model/mesh";
    mime["meta4"] = "application/metalink4+xml";
    mime["metalink"] = "application/metalink+xml";
    mime["mets"] = "application/mets+xml";
    mime["mft"] = "application/rpki-manifest";
    mime["mid"] = "audio/midi";
    mime["midi"] = "audio/midi";
    mime["mime"] = "message/rfc822";
    mime["mj2"] = "video/mj2";
    mime["mjp2"] = "video/mj2";
    mime["mjs"] = "application/javascript";
    mime["mml"] = "text/mathml";
    mime["mods"] = "application/mods+xml";
    mime["mov"] = "video/quicktime";
    mime["mp2"] = "audio/mpeg";
    mime["mp21"] = "application/mp21";
    mime["mp2a"] = "audio/mpeg";
    mime["mp3"] = "audio/mpeg";
    mime["mp4"] = "video/mp4";
    mime["mp4a"] = "audio/mp4";
    mime["mp4s"] = "application/mp4";
    mime["mp4v"] = "video/mp4";
    mime["mpd"] = "application/dash+xml";
    mime["mpe"] = "video/mpeg";
    mime["mpeg"] = "video/mpeg";
    mime["mpg"] = "video/mpeg";
    mime["mpg4"] = "video/mp4";
    mime["mpga"] = "audio/mpeg";
    mime["mrc"] = "application/marc";
    mime["mrcx"] = "application/marcxml+xml";
    mime["ms"] = "text/troff";
    mime["mscml"] = "application/mediaservercontrol+xml";
    mime["msh"] = "model/mesh";
    mime["msi"] = "application/octet-stream";
    mime["msm"] = "application/octet-stream";
    mime["msp"] = "application/octet-stream";
    mime["mxf"] = "application/mxf";
    mime["mxml"] = "application/xv+xml";
    mime["n3"] = "text/n3";
    mime["nb"] = "application/mathematica";
    mime["oda"] = "application/oda";
    mime["oga"] = "audio/ogg";
    mime["ogg"] = "audio/ogg";
    mime["ogv"] = "video/ogg";
    mime["ogx"] = "application/ogg";
    mime["omdoc"] = "application/omdoc+xml";
    mime["onepkg"] = "application/onenote";
    mime["onetmp"] = "application/onenote";
    mime["onetoc"] = "application/onenote";
    mime["onetoc2"] = "application/onenote";
    mime["opf"] = "application/oebps-package+xml";
    mime["otf"] = "font/otf";
    mime["owl"] = "application/rdf+xml";
    mime["oxps"] = "application/oxps";
    mime["p10"] = "application/pkcs10";
    mime["p7c"] = "application/pkcs7-mime";
    mime["p7m"] = "application/pkcs7-mime";
    mime["p7s"] = "application/pkcs7-signature";
    mime["p8"] = "application/pkcs8";
    mime["pdf"] = "application/pdf";
    mime["pfr"] = "application/font-tdpfr";
    mime["pgp"] = "application/pgp-encrypted";
    mime["pkg"] = "application/octet-stream";
    mime["pki"] = "application/pkixcmp";
    mime["pkipath"] = "application/pkix-pkipath";
    mime["pls"] = "application/pls+xml";
    mime["png"] = "image/png";
    mime["prf"] = "application/pics-rules";
    mime["ps"] = "application/postscript";
    mime["pskcxml"] = "application/pskc+xml";
    mime["qt"] = "video/quicktime";
    mime["raml"] = "application/raml+yaml";
    mime["rdf"] = "application/rdf+xml";
    mime["rif"] = "application/reginfo+xml";
    mime["rl"] = "application/resource-lists+xml";
    mime["rld"] = "application/resource-lists-diff+xml";
    mime["rmi"] = "audio/midi";
    mime["rnc"] = "application/relax-ng-compact-syntax";
    mime["rng"] = "application/xml";
    mime["roa"] = "application/rpki-roa";
    mime["roff"] = "text/troff";
    mime["rq"] = "application/sparql-query";
    mime["rs"] = "application/rls-services+xml";
    mime["rsd"] = "application/rsd+xml";
    mime["rss"] = "application/rss+xml";
    mime["rtf"] = "application/rtf";
    mime["rtx"] = "text/richtext";
    mime["s3m"] = "audio/s3m";
    mime["sbml"] = "application/sbml+xml";
    mime["scq"] = "application/scvp-cv-request";
    mime["scs"] = "application/scvp-cv-response";
    mime["sdp"] = "application/sdp";
    mime["ser"] = "application/java-serialized-object";
    mime["setpay"] = "application/set-payment-initiation";
    mime["setreg"] = "application/set-registration-initiation";
    mime["sgi"] = "image/sgi";
    mime["sgm"] = "text/sgml";
    mime["sgml"] = "text/sgml";
    mime["shex"] = "text/shex";
    mime["shf"] = "application/shf+xml";
    mime["shtml"] = "text/html";
    mime["sig"] = "application/pgp-signature";
    mime["sil"] = "audio/silk";
    mime["silo"] = "model/mesh";
    mime["slim"] = "text/slim";
    mime["slm"] = "text/slim";
    mime["smi"] = "application/smil+xml";
    mime["smil"] = "application/smil+xml";
    mime["snd"] = "audio/basic";
    mime["so"] = "application/octet-stream";
    mime["spp"] = "application/scvp-vp-response";
    mime["spq"] = "application/scvp-vp-request";
    mime["spx"] = "audio/ogg";
    mime["sru"] = "application/sru+xml";
    mime["srx"] = "application/sparql-results+xml";
    mime["ssdl"] = "application/ssdl+xml";
    mime["ssml"] = "application/ssml+xml";
    mime["stk"] = "application/hyperstudio";
    mime["styl"] = "text/stylus";
    mime["stylus"] = "text/stylus";
    mime["svg"] = "image/svg+xml";
    mime["svgz"] = "image/svg+xml";
    mime["t"] = "text/troff";
    mime["t38"] = "image/t38";
    mime["tei"] = "application/tei+xml";
    mime["teicorpus"] = "application/tei+xml";
    mime["text"] = "text/plain";
    mime["tfi"] = "application/thraud+xml";
    mime["tfx"] = "image/tiff-fx";
    mime["tif"] = "image/tiff";
    mime["tiff"] = "image/tiff";
    mime["tr"] = "text/troff";
    mime["ts"] = "video/mp2t";
    mime["tsd"] = "application/timestamped-data";
    mime["tsv"] = "text/tab-separated-values";
    mime["ttc"] = "font/collection";
    mime["ttf"] = "font/ttf";
    mime["ttl"] = "text/turtle";
    mime["u8dsn"] = "message/global-delivery-status";
    mime["u8hdr"] = "message/global-headers";
    mime["u8mdn"] = "message/global-disposition-notification";
    mime["u8msg"] = "message/global";
    mime["uri"] = "text/uri-list";
    mime["uris"] = "text/uri-list";
    mime["urls"] = "text/uri-list";
    mime["vcard"] = "text/vcard";
    mime["vrml"] = "model/vrml";
    mime["vtt"] = "text/vtt";
    mime["vxml"] = "application/voicexml+xml";
    mime["war"] = "application/java-archive";
    mime["wasm"] = "application/wasm";
    mime["wav"] = "audio/wav";
    mime["weba"] = "audio/webm";
    mime["webm"] = "video/webm";
    mime["webmanifest"] = "application/manifest+json";
    mime["webp"] = "image/webp";
    mime["wgt"] = "application/widget";
    mime["wmf"] = "image/wmf";
    mime["woff"] = "font/woff";
    mime["woff2"] = "font/woff2";
    mime["wrl"] = "model/vrml";
    mime["wsdl"] = "application/wsdl+xml";
    mime["wspolicy"] = "application/wspolicy+xml";
    mime["x3d"] = "model/x3d+xml";
    mime["x3db"] = "model/x3d+binary";
    mime["x3dbz"] = "model/x3d+binary";
    mime["x3dv"] = "model/x3d+vrml";
    mime["x3dvz"] = "model/x3d+vrml";
    mime["x3dz"] = "model/x3d+xml";
    mime["xaml"] = "application/xaml+xml";
    mime["xdf"] = "application/xcap-diff+xml";
    mime["xdssc"] = "application/dssc+xml";
    mime["xenc"] = "application/xenc+xml";
    mime["xer"] = "application/patch-ops-error+xml";
    mime["xht"] = "application/xhtml+xml";
    mime["xhtml"] = "application/xhtml+xml";
    mime["xhvml"] = "application/xv+xml";
    mime["xm"] = "audio/xm";
    mime["xml"] = "application/xml";
    mime["xop"] = "application/xop+xml";
    mime["xpl"] = "application/xproc+xml";
    mime["xsd"] = "application/xml";
    mime["xsl"] = "application/xml";
    mime["xslt"] = "application/xslt+xml";
    mime["xspf"] = "application/xspf+xml";
    mime["xvm"] = "application/xv+xml";
    mime["xvml"] = "application/xv+xml";
    mime["yaml"] = "text/yaml";
    mime["yang"] = "application/yang";
    mime["yin"] = "application/yin+xml";
    mime["yml"] = "text/yaml";
    mime["zip"] = "application/zip";
    mime["ico"] = "image/x-icon";
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
    return (mime.at(requestedExtension)); // searching extension in mime // at throw an exception if it's not find
}

std::string MediaType::getMediaExtension(std::string requestedType)
{
    if (requestedType.find('/') == std::string::npos)
        return (NULL);
    requestedType.erase(std::remove_if(requestedType.begin(), requestedType.end(), ::isspace),
        requestedType.end()); // remove all space
    for(std::map<std::string, std::string>::iterator it = mime.begin(); it != mime.end(); ++it)
    {
        if(mime[(*it).first] == requestedType)
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
                    for(std::map<std::string, std::string>::iterator it = mime.begin(); it != mime.end(); ++it)
                    {
                        std::string full_extension = first + '/' + mime[(*it).first].substr(mime[(*it).first].find('/') + 1);
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
    for(std::map<std::string, std::string>::iterator it = mime.begin(); it != mime.end(); ++it)
    {
        if(mime[(*it).first] == requestedType)
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


/* ************************************************************************** */