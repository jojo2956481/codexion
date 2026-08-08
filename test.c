if (data->scheduler[0] == 'l')
{
	for (int i = count; i > 0; i--)
		data->queue.coder_ids[i] = data->queue.coder_ids[i - 1];
	data->queue.coder_ids[0] = c->id;
}

clean_up.c :

/* La file d'attente (lock/cond/coder_ids) n'est initialisee que si
 * init_queue() a reussi (coder_ids non-NULL dans ce cas), on ne la
 * detruit donc que si elle existe reellement. */

/* Les dongles n'ont plus de condition variable individuelle (toute
 * l'attente passe par data->queue.cond) : rien a detruire ici, la
 * fonction est conservee pour ne pas changer la signature utilisee
 * ailleurs. */

codeur.c :

/* Cas particulier : un seul codeur / un seul dongle (left_dongle ==
 * right_dongle). Il ne peut jamais reunir deux dongles distincts pour
 * compiler : il prend le seul dongle disponible, attend l'arret de la
 * simulation, puis le relache. On verrouille directement le mutex du
 * dongle (et non via take_dongles, qui verrouillerait deux fois le
 * meme mutex et bloquerait pour toujours). */

dongle.c :

/* Attend au plus ms millisecondes sur cond (mutex deja tenu par l'appelant).
 * Permet de se reveiller meme si personne ne broadcast, pour re-verifier
 * une condition dependante du temps (cooldown des dongles). */

/* LIFO : le nouveau passe devant tout le monde (en tete de file). */

/* EDF : insertion triee par ordre croissant de "last_meal" (deadline
 * approchant en premier), puisque tous les codeurs partagent le meme
 * time_to_burnout la comparaison sur last_meal equivaut a comparer
 * la deadline reelle. */

/* queue.lock est deja tenu par l'appelant. Un seul passage lineaire sur
 * la file (dans l'ordre de priorite fifo/lifo/edf) suffit a distribuer
 * TOUS les couples de dongles actuellement disponibles, pas seulement
 * le premier de la file : chaque paire accordee est retiree/verrouillee
 * immediatement, donc les demandes suivantes en conflit avec elle
 * echoueront naturellement au meme passage (priorite respectee), tandis
 * que les demandes portant sur des dongles totalement disjoints sont
 * accordees dans la foulee, sans attendre un tour de boucle separe.
 * Cela evite qu'avec beaucoup de codeurs, chaque thread reveille doive
 * relancer son propre scan complet sous le meme verrou global (ce qui
 * degraderait en O(n^2) et faisait perdre un temps precieux face au
 * burnout). */

/* Calcule l'instant (en ms) auquel il est pertinent de se reveiller :
 * - si un des deux dongles est actuellement verrouille par quelqu'un
 *   d'autre, on ne peut compter que sur le broadcast de release_dongles
 *   -> court sondage de securite ;
 * - sinon, les dongles sont juste en cooldown : on se reveille pile a
 *   l'instant ou le cooldown expire, pour eviter tout decalage inutile
 *   qui s'accumulerait au fil des cycles. */

/* Prend les deux dongles (gauche + droite) de facon atomique : soit les
 * deux sont attribues d'un coup, soit aucun ne l'est. Aucun thread ne
 * peut donc jamais detenir un seul dongle en attendant l'autre : c'est
 * ce qui empeche le deadlock classique des philosophes. */
