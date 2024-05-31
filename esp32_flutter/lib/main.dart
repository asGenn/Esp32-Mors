import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:esp32/firebase_options.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:flutter/material.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      home: ChatScreen(),
    );
  }
}

class ChatScreen extends StatefulWidget {
  const ChatScreen({super.key});

  @override
  _ChatScreenState createState() => _ChatScreenState();
}

class _ChatScreenState extends State<ChatScreen> {
  final TextEditingController _controller = TextEditingController();
  final CollectionReference _messagesRef =
      FirebaseFirestore.instance.collection('messages');
  bool _canSendMessage = true;

  void _sendMessage() {
    final message = _controller.text;
    if (message.isNotEmpty && _canSendMessage) {
      _messagesRef
          .add({'text': message, 'timestamp': FieldValue.serverTimestamp()});
      _controller.clear();
    }
  }

  @override
  Widget build(BuildContext context) {
    Size size = MediaQuery.of(context).size;
    return Scaffold(
      appBar: AppBar(
        centerTitle: true,
        title: const Text('ESP32 ile Mesajlaşma Uygulaması'),
      ),
      body: Column(
        mainAxisSize: MainAxisSize.max,
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          Image.asset(
            'assets/foto.jpg',
            width: size.width * 0.5,
            height: size.height * 0.3,
          ),
          StreamBuilder<QuerySnapshot>(
            stream:
                _messagesRef.orderBy('timestamp', descending: true).snapshots(),
            builder: (context, snapshot) {
              if (!snapshot.hasData) {
                return const Center(child: CircularProgressIndicator());
              }
              final messages = snapshot.data!.docs;

              WidgetsBinding.instance.addPostFrameCallback((_) {
                setState(() {
                  _canSendMessage = messages.length != 1;
                });
              });

              return messages.isEmpty
                  ? Column(
                      mainAxisAlignment: MainAxisAlignment.end,
                      mainAxisSize: MainAxisSize.max,
                      children: [
                        Padding(
                          padding: EdgeInsets.only(
                            left: size.width * 0.3,
                            right: size.width * 0.3,
                            bottom: size.height * 0.05,
                          ),
                          child: Row(
                            children: [
                              Expanded(
                                child: TextField(
                                  controller: _controller,
                                  decoration: const InputDecoration(
                                    labelText: 'Mesajınızı yazın...',
                                  ),
                                  onSubmitted: (_) => _sendMessage(),
                                ),
                              ),
                              IconButton(
                                icon: const Icon(Icons.send),
                                onPressed:
                                    _canSendMessage ? _sendMessage : null,
                                color:
                                    _canSendMessage ? Colors.blue : Colors.grey,
                              ),
                            ],
                          ),
                        ),
                      ],
                    )
                  : Center(
                      child: Column(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          Container(
                            padding: const EdgeInsets.all(16.0),
                            decoration: BoxDecoration(
                              color: Colors.blue,
                              borderRadius: BorderRadius.circular(8.0),
                            ),
                            child: Text(
                              '${messages[0]['text']}',
                              style: const TextStyle(color: Colors.white),
                              textAlign: TextAlign.center,
                            ),
                          ),
                          SizedBox(height: size.height * 0.02),
                          const Text(
                            "Mesajın gönderildi. Okuması için bekleniyor...",
                            style: TextStyle(color: Colors.red),
                          ),
                          SizedBox(height: size.height * 0.2),
                        ],
                      ),
                    );
            },
          ),
        ],
      ),
    );
  }
}
