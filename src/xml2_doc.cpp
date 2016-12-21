#include <Rcpp.h>
using namespace Rcpp;

#include <libxml/parser.h>
#include <libxml/HTMLparser.h>
#include "xml2_types.h"
#include "xml2_utils.h"

// [[Rcpp::export]]
Rcpp::IntegerVector xml_parse_options() {
  const char * names[] = {
    "RECOVER",
    "NOENT",
    "DTDLOAD",
    "DTDATTR",
    "DTDVALID",
    "NOERROR",
    "NOWARNING",
    "PEDANTIC",
    "NOBLANKS",
    "SAX1",
    "XINCLUDE",
    "NONET",
    "NODICT",
    "NSCLEAN",
    "NOCDATA",
    "NOXINCNODE",
    "COMPACT",
    "OLD10",
    "NOBASEFIX",
    "HUGE",
    "OLDSAX",
    "IGNORE_ENC",
    "BIG_LINES",
  };

  const int values[] = {
    XML_PARSE_RECOVER,
    XML_PARSE_NOENT,
    XML_PARSE_DTDLOAD,
    XML_PARSE_DTDATTR,
    XML_PARSE_DTDVALID,
    XML_PARSE_NOERROR,
    XML_PARSE_NOWARNING,
    XML_PARSE_PEDANTIC,
    XML_PARSE_NOBLANKS,
    XML_PARSE_SAX1,
    XML_PARSE_XINCLUDE,
    XML_PARSE_NONET,
    XML_PARSE_NODICT,
    XML_PARSE_NSCLEAN,
    XML_PARSE_NOCDATA,
    XML_PARSE_NOXINCNODE,
    XML_PARSE_COMPACT,
    XML_PARSE_OLD10,
    XML_PARSE_NOBASEFIX,
    XML_PARSE_HUGE,
    XML_PARSE_OLDSAX,
    XML_PARSE_IGNORE_ENC,
    XML_PARSE_BIG_LINES,
  };

  const char * descriptions[] = {
    "recover on errors",
    "substitute entities",
    "load the external subset",
    "default DTD attributes",
    "validate with the DTD",
    "suppress error reports",
    "suppress warning reports",
    "pedantic error reporting",
    "remove blank nodes",
    "use the SAX1 interface internally",
    "Implement XInclude substitition",
    "Forbid network access",
    "Do not reuse the context dictionary",
    "remove redundant namespaces declarations",
    "merge CDATA as text nodes",
    "do not generate XINCLUDE START/END nodes",
    "compact small text nodes; no modification of the tree allowed afterwards (will possibly crash if you try to modify the tree)",
    "parse using XML-1.0 before update 5",
    "do not fixup XINCLUDE xml:base uris",
    "relax any hardcoded limit from the parser",
    "parse using SAX2 interface before 2.7.0",
    "ignore internal document encoding hint",
    "Store big lines numbers in text PSVI field",
  };

  size_t size = sizeof(values) / sizeof(values[0]);

  Rcpp::IntegerVector out_values = Rcpp::IntegerVector(size);
  Rcpp::CharacterVector out_names = Rcpp::CharacterVector(size);
  Rcpp::CharacterVector out_descriptions = Rcpp::CharacterVector(size);
  for (int i = 0; i < size; ++i) {
    out_values[i] = values[i];
    out_names[i] = names[i];
    out_descriptions[i] = descriptions[i];
  }
  out_values.attr("names") = out_names;
  out_values.attr("descriptions") = out_descriptions;

  return out_values;
}

// [[Rcpp::export]]
XPtrDoc doc_parse_file(std::string path,
                            std::string encoding = "",
                            bool as_html = false,
                            int options = 0) {
  xmlDoc* pDoc;
  if (as_html) {
    pDoc = htmlReadFile(
      path.c_str(),
      encoding == "" ? NULL : encoding.c_str(),
      options
    );
  } else {
    pDoc = xmlReadFile(
      path.c_str(),
      encoding == "" ? NULL : encoding.c_str(),
      options
    );
  }

  if (pDoc == NULL)
    Rcpp::stop("Failed to parse %s", path);

  return XPtrDoc(pDoc);
}

// [[Rcpp::export]]
XPtrDoc doc_parse_raw(RawVector x, std::string encoding,
                      std::string base_url = "",
                      bool as_html = false,
                      int options = 0) {
  xmlDoc* pDoc;
  if (as_html) {
    pDoc = htmlReadMemory(
      (const char *) RAW(x),
      Rf_length(x),
      base_url == "" ? NULL : base_url.c_str(),
      encoding == "" ? NULL : encoding.c_str(),
      options
    );
  } else {
    pDoc = xmlReadMemory(
      (const char *) RAW(x),
      Rf_length(x),
      base_url == "" ? NULL : base_url.c_str(),
      encoding == "" ? NULL : encoding.c_str(),
      options
    );
  }

  if (pDoc == NULL)
    Rcpp::stop("Failed to parse text");

  return XPtrDoc(pDoc);
}

// [[Rcpp::export]]
XPtrNode doc_root(XPtrDoc x) {
  return XPtrNode(xmlDocGetRootElement(x.checked_get()));
}

// [[Rcpp::export]]
bool doc_has_root(XPtrDoc x) {
  return xmlDocGetRootElement(x.get()) != NULL;
}

// [[Rcpp::export]]
CharacterVector doc_url(XPtrDoc x) {
  SEXP string = (x->URL == NULL) ? NA_STRING : Rf_mkCharCE((const char*) x->URL, CE_UTF8);
  return CharacterVector(string);
}

// [[Rcpp::export]]
XPtrDoc doc_new(std::string version, std::string encoding = "UTF-8") {
  XPtrDoc x = XPtrDoc(xmlNewDoc(asXmlChar(version)));
  xmlCharEncodingHandlerPtr p = xmlFindCharEncodingHandler(encoding.c_str());
  x->encoding = xmlStrdup(reinterpret_cast<const xmlChar *>(p->name));
  return x;
}

// [[Rcpp::export]]
XPtrNode doc_set_root(XPtrDoc doc, XPtrNode root) {
  return XPtrNode(xmlDocSetRootElement(doc, root));
}
