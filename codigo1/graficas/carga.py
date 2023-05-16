import matplotlib.pyplot as plt
import seaborn as sns

# intervalos de generacion = {0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5}

# Puntos cargados a mano
# Ejemplo de carga ofrecida con generation interval = 0.1
# tiempo de simulacion = 200 ---> paquetes recibidos = 1979
#                          1 ---> x = 1979/200 = 9.895

carga_ofrecida = [9.895, 6.535, 4.945, 3.995, 3.295, 2.465, 1.95]
carga_util = [4.99, 4.99, 4.88, 3.965, 3.275, 2.465, 1.945]

sns.set_context(context='talk', font_scale=0.7)

plt.plot(carga_ofrecida, carga_util, 'ro-')
plt.xlabel('Carga ofrecida (paquetes/seg)')
plt.ylabel('Carga útil (paquetes/seg)')
plt.title('Carga útil vs Carga ofrecida')
plt.show()
plt.savefig('carga.png')