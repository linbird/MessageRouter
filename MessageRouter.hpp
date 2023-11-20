////
////  MessageRouter.hpp
////  MessageRouter
////
////  Created by Jay Zhang on 2023/10/18.
////
//
//#ifndef MessageRouter_hpp
//#define MessageRouter_hpp
//
//#define WS_API_KEY(func) const std::string KEY_##func = #func;
//
//#define WS_API_FUN(func, ...) \
//extern const std::string KEY_##func; \
//using API_##func = std::function<void(__VA_ARGS__)>;
//
//#define WS_PARAS(...) {__VA_ARGS__}
//
//#define WS_API_ADD(sender, api, func) sender.Add<API_##api>(KEY_##api, func);
//
//#define WS_API_FIND(sender, api) sender.Find<API_##api>(KEY_##api);
//
//#define WS_API_SEND(sender, api, parameters) sender.Send<API_##api>(KEY_##api, parameters);
//
//#define WS_API_REMOVE(sender, api) sender.Remove<API_##api>(KEY_##api);
//
//
//#include <map>
//#include <tuple>
//#include <string>
//#include <functional>
//#include <type_traits>
//#include "boost/any.hpp"
//
//template<typename Func>
//struct function_traits;
//
//template<typename R, typename... Args>
//struct function_traits<R(Args...)>
//{
////    using args_types = std::tuple<Args...>;
//    using args_types = std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;
//    using result_type = R;
//};
//
//template<typename R, typename... Args>
//struct function_traits<R(*)(Args...)> : function_traits<R(Args...)>{};
//
//template <typename R, typename... Args>
//struct function_traits<std::function<R(Args...)>> : function_traits<R(Args...)>{};
//
//template <typename Functor, typename... Args, std::size_t... I>
//decltype(auto) call_helper(Functor func, std::tuple<Args...> &&params, std::index_sequence<I...>)
//{
//    return func(std::get<I>(params)...);
//}
//
//class MessageRouter
//{
//public:
//    MessageRouter() = default;
//    MessageRouter(const MessageRouter&) = delete;
//    MessageRouter& operator = (const MessageRouter&) = delete;
//        
////    template<typename R, typename... Args>
////    void Add(const std::string& topic, auto&& f)
////    {
////        using function_type = std::function<R(Args...)>;
////        std::string message_type = topic + typeid(function_type).name();
////        m_map.emplace(std::move(message_type), std::forward<function_type>(f));
////    }
////
////    template<typename R, typename... Args>
////    void Remove(const std::string& topic)
////    {
////        using function_type = std::function<R(Args...)>;
////        std::string message_type =topic +typeid(function_type).name();
////        auto range = m_map.equal_range(message_type);
////        m_map.erase(range.first, range.second);
////    }
////
////    template<typename R, typename... Args>
////    void Send(const std::string& topic, Args&&... args)
////    {
////        using function_type = std::function<R(Args...)>;
////        std::string message_type = topic + typeid(function_type).name();
////        auto range = m_map.equal_range(message_type);
////        for (Iterater it = range.first; it != range.second; ++it)
////        {
////            auto f = boost::any_cast<function_type>(it->second);
////            f(std::forward<Args>(args)...);
////        }
////    }
////
////    template<typename R, typename... Args>
////    bool Find(const std::string& topic)
////    {
////        using function_type = std::function<R(Args...)>;
////        std::string message_type = topic + typeid(function_type).name();
////        auto range = m_map.equal_range(message_type);
////        return range.first != range.second;
////    }
//    
//    template<typename F>
//    void Add(const std::string& topic, F&& f)
//    {
//        std::string message_type = topic + typeid(F).name();
//        m_map.emplace(std::move(message_type), std::forward<F>(f));
//    }
//        
//    template<typename F>
//    void Remove(const std::string& topic)
//    {
//        std::string message_type = topic + typeid(F).name();
//        auto range = m_map.equal_range(message_type);
//        m_map.erase(range.first, range.second);
//    }
//    
//    template<typename F>
//    void Send(const std::string& topic, typename function_traits<F>::args_types&& args)
//    {
//        using func_args = typename function_traits<F>::args_types;
//
//        std::string message_type = topic + typeid(F).name();
//        auto range = m_map.equal_range(message_type);
//        for (Iterater it = range.first; it != range.second; ++it)
//        {
//            auto f = boost::any_cast<F>(it->second);
//            constexpr auto size = std::tuple_size<func_args>{};
//            using indices = std::make_index_sequence<size>;
//            call_helper(f, std::forward<func_args>(args), indices{});
//        }
//    }
//    
////    template<typename F>
////    void Send(const std::string& topic)
////    {
////        std::string message_type = topic + typeid(F).name();
////        auto range = m_map.equal_range(message_type);
////        for (Iterater it = range.first; it != range.second; ++it)
////        {
////            auto f = boost::any_cast<F>(it->second);
////            f();
////        }
////    }
//        
//    template<typename F>
//    bool Find(const std::string& topic)
//    {
//        std::string message_type = topic + typeid(F).name();
//        auto range = m_map.equal_range(message_type);
//        return range.first != range.second;
//    }
//    
//private:
//    void test() {}
//
//private:
//    std::multimap<std::string, boost::any> m_map;
//    typedef std::multimap<std::string, boost::any>::iterator Iterater;
//};
//
//#endif /* MessageRouter_hpp */
