import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# intervalos de generacion = {0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5}

# Puntos cargados a mano
# Ejemplo de carga ofrecida con generation interval = 0.1
# tiempo de simulacion = 200 ---> paquetes recibidos = 1979
#                          1 ---> x = 1979/200 = 9.895

intervalos_generacion = [0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5]
carga_ofrecida = [9.895, 6.535, 4.945, 3.995, 3.295, 2.465, 1.95]
carga_util = [4.99, 4.99, 4.88, 3.965, 3.275, 2.465, 1.945]

carga_util = np.array(carga_util)
carga_ofrecida = np.array(carga_ofrecida)

aprovechamiento = carga_util / carga_ofrecida 

sns.set_context(context='talk', font_scale=0.7)

plt.plot(intervalos_generacion, aprovechamiento, 'ro-')
plt.xlabel('Intervalos de generacion (seg)')
plt.ylabel('Paquetes recibidos / paquetes enviados')
plt.title('Aprovechamiento vs Intervalos de generacion')
plt.show()
plt.savefig('aprovechamiento.png')