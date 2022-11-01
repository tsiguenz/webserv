#include "test.h"
#include "webserv.h"
# include "MediaType.hpp"
# include "Server.hpp"
#include "ErrorHandler.hpp"

int	main() {
	std::cout << "TEST main " << std::endl;
	std::string request = "GET /index.html HTTP/1.1\r\n";
    			request += "Host: localhost:1024\r\n";
				request += "Connection: keep-alive\r\n";
				request += "sec-ch-ua: \"Google Chrome\";v=\"105\", \"Not)A;Brand\";v=\"8\", \"Chromium\";v=\"105\"\r\n";
				request += "sec-ch-ua-mobile: ?ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd0\r\n"; // AGEEEEEEEEEEEEEEEEEEEEEERERRRRRRRRRRRRRRRR
				request += "sec-ch-ua-platform: \"Linux\"\r\n";
				request += "Upgrade-Insecure-Requests: 1\r\n";
				request += "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36\r\n";
				request += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n";
				request += "Sec-Fetch-Site: none\r\n";
				request += "Sec-Fetch-Mode: navigate\r\n";
				request += "Sec-Fetch-User: ?1\r\n";
				request += "Sec-Fetch-Dest: document\r\n";
				request += "Accept-Encoding: gzip, deflate, br\r\n";
				request += "Accept-Language: en-US,en;q=0.9\r\n";
				request += "\r\n";
				request += "body be like";
    
	std::cout << request;
	Response lol(request);
	lol.printResponse();
	// Request lol(request);
	// if (lol.badRequest == true) {
	// 	std::cout << "400 \n";
	// 	return 1;
	// }
	// lol.printRequest();
	// Response mdr(lol);
	// mdr.printResponse();

	// ---------------------------------------------------------//
	
	// MediaType lol;

	// std::cout << "quand on envoie 'png' : "<< lol.getMediaType("png") << std::endl;
	// std::cout << "quand on envoie ' text/html;charset=utf-8 ' : "<< lol.isTypeSupported(" text/html;charset=utf-8 ") << std::endl;
	// std::cout << "quand on envoie ' application/json' : "<< lol.isTypeSupported(" application/json") << std::endl;
	// std::cout << "quand on envoie accepted png pour  text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9; 'png' : "<< lol.isInAccepted("png", " text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n") << std::endl;

	// std::cout << "quand on envoie ' lol/image.jpg' : "<< lol.getMediaType("lol/image.jpg") << std::endl; //new

	// ---------------------------------------------------------//

	// ErrorHandler lol;
	// std::cout << lol.generateErrorHtml(404);
	return 0;
}
