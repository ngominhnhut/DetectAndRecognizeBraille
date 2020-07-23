net = patternnet([320,212,102]);
net = train(net,input,target);
view(net)
y = net(input);
perf = perform(net,target,y);
classes = vec2ind(y);
