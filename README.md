Much like a dwarf, out of the blue I decided to build a relatively simple political simulator solely in C.

4.75 hours later, I'm done! And while it wasn't particular painful, a large chunk of that time was me hunting for bugs, discovering that passing structures as parameters actually passes in a _copy_ rather than the structure itself, and discovering that having an array of pointers is actually pretty complex, and leads to its own host of errors.

The political simulator, though simple, is relatively fun to watch. The actors have a simple goal of maximizing their population. Every turn, three things happen. Each actor takes a national action, a random event may occur (disease, a secessionist movement, and a technological development), and their population grows or shrinks based on their fertility level, which broadly represents domestic infrastructure. A national action is essentially one of two decisions: spend that turn developing their domestic infrastructure, which increases their fertility by some multiple, or go to war with a weaker neighbor for the possibility of the spoils of war. If a state looks for a weaker neighbor to invade, that takes up their turn, even if no such weaker neighbor is found and so they technically never go to war.

Additionally, once the state is old enough (Five days... since I chose days as my demarcating unit), it'll modify its strategy to try to maximize its population. Consequently, each state has a memory of (roughly) what worked and what didn't work, and based on this memory they decide what to do next.



ORIGINAL STORY (WITH SOME INITIAL SCREENSHOTS AND INTERPRETATION) FOUND HERE: https://www.facebook.com/KwontomPhysics/posts/1106256502726747