/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ml_server.cpp
 * Author: ubuntu
 * 
 * Created on May 14, 2020, 3:11 PM
 */

#include "vs_server.h"
#include "vs_processing.h"

vs_server::vs_server(vs_config *cfg) {
    config = cfg;
}

vs_server::~vs_server() {
}

string vs_server::get_socket_key(websocketpp::connection_hdl hdl){
    server::connection_ptr Connection = ws_server.get_con_from_hdl( hdl );
    string socketKey = Connection->get_request_header("Sec-WebSocket-Key");
    return socketKey;
}

string vs_server::create_frame_response(vs_server* s, VideoCapture videoCapture){
    string base64_data = s->get_frame_content(videoCapture);
    if(base64_data.length() == 0)
        return "";
    return create_response_data(GET_FRAME_DATA, 0, base64_data);
}

void vs_server::on_message(vs_server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    
    string payloadData = msg->get_payload();
    string data_content = payloadData;
    string socketKey = s->get_socket_key(hdl);
    if(string_util::start_width(payloadData, "get-frame")){//confirm: get-frame
        if(s->client_video_captures.count(socketKey) > 0){
            data_content = vs_server::create_frame_response(s, s->client_video_captures[socketKey]);
            if(data_content.length() <= 0)
            {
                data_content = vs_server::create_response_data(GET_FRAME_DATA, 0, EOF_VIDEO_DATA);
                s->send(hdl, data_content, msg->get_opcode());
                s->pause_reading(hdl);
                s->close_client(hdl, websocketpp::close::status::normal, "END OF VIDEO");
                return;
            }
        }
    }else if(string_util::start_width(payloadData, "get-info")){//confirm: get-frame
        if(s->client_video_captures.count(socketKey) > 0){
            data_content = create_info_response(s->client_video_captures[socketKey]);
        }
    }
    
    try {
        s->send(hdl, data_content, msg->get_opcode());
        data_content.clear();
    } catch (websocketpp::exception const & e) {
        std::cout << "Echo failed because: "
                << "(" << e.what() << ")" << std::endl;
    }
}

void vs_server::send_to_all_clients(server *s, string data){
    for (auto it : client_connections) {
        s->send(it, data, websocketpp::frame::opcode::text);
    }
}

void vs_server::on_open(vs_server* s, websocketpp::connection_hdl hdl) {
    s->add_client(hdl);
    cout << "open connection from " << hdl.lock().get() << endl;
    server::connection_ptr Connection = s->get_con_from_hdl( hdl );
    string socketKey = s->get_socket_key(hdl);
    string requestUri = Connection->get_request().get_uri();
    if(!string_util::start_width(requestUri, "/video/"))
    {
        s->close_client(hdl, websocketpp::close::status::normal, "INVALID REQUEST");  
        return;
    }else{
        string videoPath = requestUri.substr(7);// /video/
        cout<<socketKey<<" PATH: "<<videoPath<<endl;
        VideoCapture videoCapture = VideoCapture(videoPath, cv::CAP_ANY);
        if(!videoCapture.isOpened()){
            videoCapture.release();
            s->close_client(hdl, websocketpp::close::status::normal, "INVALID URL");  
        }
        else{
            videoCapture.set(CAP_PROP_BUFFERSIZE, 3); 
            s->client_video_captures[socketKey] = videoCapture;
        }
    }
}

void vs_server::on_close(vs_server* s, websocketpp::connection_hdl hdl) {
    s->remove_client(hdl);
    string socketKey = s->get_socket_key(hdl);
    if(s->client_video_captures.count(socketKey) > 0){
        s->client_video_captures[socketKey].release();
        s->client_video_captures.erase(socketKey);
    }
}

string vs_server::get_frame_content(VideoCapture videoCapture) {
    Mat currentFrame;
    videoCapture >> currentFrame;
    for(int i = 0; i < config->step; i++){
        videoCapture.grab();
    }
    if (!currentFrame.empty()) {
        if(config->resized != 1)
            cv::resize(currentFrame, currentFrame, cv::Size(), config->resized, config->resized);
        string content = vs_processing::to_base64(currentFrame);
        currentFrame.release();
        return content;
    }else 
        videoCapture.release();
    return "";
}

void vs_server::start_listen(){
    try {
        ws_server.set_reuse_addr(true);
        // Set logging settings
        ws_server.set_max_http_body_size(10 * 1024 * 1024);
        ws_server.set_max_message_size(10 * 1024 * 1024);
        
        ws_server.set_access_channels(websocketpp::log::alevel::none);
        ws_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        ws_server.init_asio();

        ws_server.set_open_handler(bind(&on_open, this, ::_1));
        ws_server.set_close_handler(bind(&on_close, this, ::_1));
        // Register our message handler
        ws_server.set_message_handler(bind(&on_message, this, ::_1, ::_2));

        // Listen on port 4121
        ws_server.listen(config->listen_port);

        // Start the server accept loop
        ws_server.start_accept();
        cout<<"START WEBSOCKET SERVER ON PORT: "<<config->listen_port<<endl;
        // Start the ASIO io_service run loop
        ws_server.run();
        
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}